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
#include <QNetworkConfigurationManager>
#include <memory>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppRosterIq.h"
#include "chat-adapter/chatbot.h"
#include "chat-adapter/historyhelper.h"

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
     * Constructs a XMPP chatbot with ID \a chatbotId and parent object \a parent
     */
    XmppChatbot(const QString &chatbotId, QObject *parent = 0);

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
     * \copydoc Chatbot::connectToServer(const QString&, const QString&, const QString&)
     */
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &domain);

    /**
     * \copydoc Chatbot::disconnectFromServer()
     */
    virtual void disconnectFromServer();

    /**
     * \copydoc Chatbot::isConnected()
     */
    virtual bool isConnected() const;

    /**
     * \copydoc Chatbot::username()
     */
    virtual const QString &username();

    /**
     * \copydoc Chatbot::domain()
     */
    virtual const QString &domain();

    /**
     * \copydoc Chatbot::setAI()
     */
    virtual void setAI(ChatbotAI *ai);

    /**
     * \copydoc Chatbot::AI()
     */
    virtual ChatbotAI *AI();

    /**
     * \copydoc Chatbot::roster()
     */
    virtual ContactInfoList roster() const;

    /**
     * \copydoc Chatbot::setBlackListRoster()
     */
    virtual void setBlackListRoster(const ContactInfoList &blackList);

    /**
     * \copydoc Chatbot::blackListRoster()
     */
    virtual ContactInfoList blackListRoster() const;

    /**
     * \copydoc Chatbot::setHistoryFilename()
     */
    virtual void setHistoryFilename(const QString &filename);

    /**
     * \copydoc Chatbot::historyFilename()
     */
    virtual QString historyFilename() const;

    /**
     * \copydoc Chatbot::chatHistory()
     */
    virtual const Cmn::Conversation &chatHistory() const;

    /**
     * \copydoc Chatbot::setChatHistory()
     */
    virtual void setChatHistory(const Cmn::Conversation &conv);

    /**
     * \copydoc Chatbot::clearHistory()
     */
    virtual void clearHistory();

signals:

    /**
     * \copydoc Chatbot::connected()
     */
    void connected();

    /**
     * \copydoc Chatbot::disconnected()
     */
    void disconnected();

    /**
     * \copydoc Chatbot::error()
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
    QString m_user;
    QString m_domain;
    QString m_name;
    HistoryHelper m_history;

    bool isInBlackList(const QString &jid);

    virtual bool tlsRequired() const;

private slots:
    void emitLocalError(QXmppClient::Error);
    void onOnlineStateChanged(bool isOnline);
private:
    XmppChatbot(XmppChatbot&);
    XmppChatbot& operator=(XmppChatbot&);

    std::auto_ptr<ChatbotAI> m_ai;
    QHash<QString, QXmppVCardIq> m_vCards;
    QMutex *m_contactInfoMutex;
    QMutex *m_rosterMutex;
    QMutex *m_aiMutex;
    bool m_isConnected;
    bool m_rosterHasChanged;
    mutable ContactInfoList m_roster;
    ContactInfoList m_blackListRoster;
    QSet<QString> m_blackListSet;
    uint m_connStartTime;
    QNetworkConfigurationManager m_netMgr;

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

