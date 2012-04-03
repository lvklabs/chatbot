/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Botmaster.
 *
 * LVK Botmaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Botmaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Botmaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_CA_XMPPCHATBOT_H
#define LVK_CA_XMPPCHATBOT_H

#include <QObject>
#include <QHash>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "chatbot.h"

class QXmppVCardIq;
class QMutex;
class QWaitCondition;

namespace Lvk
{

namespace CA
{

class ContactInfo;

/**
 * \brief Chatbot for XMPP chat servers
 */

class XmppChatbot : public Chatbot
{
    Q_OBJECT

public:
    XmppChatbot(QObject *parent = 0);

    ~XmppChatbot();

    /**
     * \brief An enumeration for type of error.
     */
    enum Error {
        SocketError,        ///< Error due to TCP socket
        KeepAliveError,     ///< Error due to no response to a keep alive
        XmppStreamError,    ///< Error due to XML stream
        InternalError       ///< Internal error
    };

    // Lvk::CA::ChatClient interface

    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);

    virtual void disconnectFromServer();

    virtual void setVirtualUser(VirtualUser *virtualUser);

    virtual VirtualUser *virtualUser();

public slots:
    void messageReceived(const QXmppMessage&);

    void vCardReceived(const QXmppVCardIq&);

signals:
    void connected();

    void disconnected();

    void error(int err);

private slots:
    void emitLocalError(QXmppClient::Error);

private:
    XmppChatbot(XmppChatbot&);
    XmppChatbot& operator=(XmppChatbot&);

    QXmppClient *m_xmppClient;
    VirtualUser *m_virtualUser;

    QHash<QString, ContactInfo> m_contactInfo;
    QMutex *m_contactInfoMutex;

    QList<QXmppMessage> m_messageQueue;
    QMutex *m_messageQueueMutex;

    void replyMessage(const QXmppMessage &msg, const ContactInfo &info);

    ContactInfo getContactInfo(const QString &bareJid);
    void requestContactInfo(const QXmppMessage &msg);
    void replyQueuedMessages(const ContactInfo &info);

    Error convertToLocalError(QXmppClient::Error err);
};

} //namespace CA

} //namespace Lvk

#endif // LVK_CA_XMPPCHATBOT_H
