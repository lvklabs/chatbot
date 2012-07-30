/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_CA_XMPPCHATBOT_H
#define LVK_CA_XMPPCHATBOT_H

#include <QObject>
#include <QHash>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppRosterIq.h"
#include "chat-adapter/chatbot.h"

class QXmppVCardIq;
class QMutex;
class QWaitCondition;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace CA
{

/// \ingroup Lvk
/// \addtogroup CA
/// @{

class ContactInfo;

/**
 * \brief The XmppChatbot class provides a chatbot for XMPP chat servers.
 *
 * XMPP is also known as Jabber.
 */

class XmppChatbot : public Chatbot
{
    Q_OBJECT

public:

    /**
     * Constructs a XMPP chatbot with parent object \a parent
     */
    XmppChatbot(QObject *parent = 0);

    /**
     * Destroys the chatbot and disconnects from the chat server if nescessary.
     */
    ~XmppChatbot();

    /**
     * \brief An enumeration for type of error.
     */
    enum Error {
        SocketError,         ///< Error due to TCP socket
        KeepAliveError,      ///< Error due to no response to a keep alive
        XmppStreamError,     ///< Error due to XML stream
        SSLNotSupportedError,///< Error due to SSL not supported on the system
        UnknownXmppError,    ///< Error due to unknown cause on the XMPP protocol
        InternalError        ///< XmppChatbot internal error
    };

    /**
     * Connects to the chat server at \a domain using \a user and \a password
     * Emits \a connected signal on success. Otherwise; emits \a connectionError.
     * If the connection ends prematurely, emits \a disconnected.
     */
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &domain);

    /**
     * Disconnects from chat server.
     */
    virtual void disconnectFromServer();

    /**
     * Sets the virtual user.
     */
    virtual void setVirtualUser(VirtualUser *virtualUser);

    /**
     * Returns the virtual user.
     */
    virtual VirtualUser *virtualUser();

    /**
     * Returns the roster. i.e. the list of all chat contacts.
     */
    virtual ContactInfoList roster() const;

    /**
     * Sets the black list roster \a blackList.
     * The black list roster is the list of contacts that the chatbot is forbidden to talk to.
     * By default, this list is empty.
     */
    virtual void setBlackListRoster(const ContactInfoList &blackList);

    /**
     * Returns the black list roster.
     * The black list roster is the list of contacts that the chatbot is forbidden to talk to.
     * By default, this list is empty.
     */
    virtual ContactInfoList blackListRoster() const;

signals:

    /**
     * This signal is emitted after invoking connectToServer() if the connection was successful.
     */
    void connected();

    /**
     * This signal is emitted after invoking connectToServer() if the connection has ended.
     */
    void disconnected();

    /**
     * This signal es emitted after invoking connectToServer() if there was an error while trying
     * to connect to the chat server.
     */
    void error(int err);

protected slots:
    virtual void onConnected();

    virtual void onDisconnected();

    virtual void onMessageReceived(const QXmppMessage &);

    virtual void onVCardReceived(const QXmppVCardIq &);

    virtual void onRosterReceived();

    virtual void onRosterChanged(const QString &);

protected:
    QXmppClient *m_xmppClient;
    VirtualUser *m_virtualUser;
    QString      m_user;
    QString      m_domain;
    QString      m_name;

    bool isInBlackList(const QString &jid);

    virtual bool tlsRequired() const;

private slots:
    void emitLocalError(QXmppClient::Error);

private:
    XmppChatbot(XmppChatbot&);
    XmppChatbot& operator=(XmppChatbot&);

    QHash<QString, QXmppVCardIq> m_vCards;
    QMutex *m_contactInfoMutex;
    QMutex *m_rosterMutex;

    bool m_isConnected;
    bool m_rosterHasChanged;
    mutable ContactInfoList m_roster;
    ContactInfoList m_blackListRoster;
    QSet<QString> m_blackListSet;

    void setupLogger();
    void connectSignals();

    virtual void connectToServer(const QString &, const QString &) {}

    ContactInfo getContactInfo(const QString &bareJid) const;

    QXmppVCardIq getVCard(const QString &bareJid);
    void requestVCard(const QString &bareJid);

    Error convertToLocalError(QXmppClient::Error err);

    void rebuildLocalRoster() const;
};

/// @}

} // namespace CA

/// @}

} //namespace Lvk

#endif // LVK_CA_XMPPCHATBOT_H
