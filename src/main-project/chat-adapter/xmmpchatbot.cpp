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

#include "xmmpchatbot.h"
#include "chatvirtualuser.h"

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppVCardManager.h"
#include "QXmppVCardIq.h"

#include <QMutex>
#include <QMutexLocker>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

QString getBareJid(const QString &from)
{
    return from.split("/").at(0);
}

} // names


//--------------------------------------------------------------------------------------------------
// XmppChatbot
//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::XmppChatbot(QObject *parent)
    : m_xmppClient(new QXmppClient(parent)),
      m_virtualUser(0),
      m_contactInfoMutex(new QMutex()),
      m_messageQueueMutex(new QMutex())
{
    // Signals

    connect(m_xmppClient, SIGNAL(messageReceived(const QXmppMessage&)),
            this, SLOT(messageReceived(const QXmppMessage&)));

    connect(&m_xmppClient->vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq&)),
            this, SLOT(vCardReceived(const QXmppVCardIq&)));

    connect(m_xmppClient, SIGNAL(connected()),    SIGNAL(connected()));
    connect(m_xmppClient, SIGNAL(disconnected()), SIGNAL(disconnected()));

    connect(m_xmppClient, SIGNAL(error(QXmppClient::Error)),
            SLOT(emitLocalError(QXmppClient::Error)));

    // Xmpp Logger

    QXmppLogger *xmppLogger = new QXmppLogger(this);
    xmppLogger->setLoggingType(QXmppLogger::FileLogging);
    xmppLogger->setLogFilePath("./xmpp.log");
    xmppLogger->setMessageTypes(QXmppLogger::AnyMessage);
    m_xmppClient->setLogger(xmppLogger);
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::~XmppChatbot()
{
    delete m_messageQueueMutex;
    delete m_contactInfoMutex;
    delete m_virtualUser;
    delete m_xmppClient;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::connectToServer(const QString &user, const QString &passwd,
                                          const QString &domain)
{
    m_xmppClient->connectToServer(user + "@" + domain, passwd);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::disconnectFromServer()
{
    m_xmppClient->disconnectFromServer();
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::setVirtualUser(Lvk::CA::VirtualUser *virtualUser)
{
    if (m_virtualUser != virtualUser) {
        delete m_virtualUser;
        m_virtualUser = virtualUser;
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::VirtualUser * Lvk::CA::XmppChatbot::virtualUser()
{
    return m_virtualUser;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::emitLocalError(QXmppClient::Error err)
{
    emit error(convertToLocalError(err));
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::Error Lvk::CA::XmppChatbot::convertToLocalError(QXmppClient::Error err)
{
    switch (err) {
    case QXmppClient::SocketError:
        return XmppChatbot::SocketError;
    case QXmppClient::KeepAliveError:
        return XmppChatbot::KeepAliveError;
    case QXmppClient::XmppStreamError:
        return XmppChatbot::XmppStreamError;
    default:
        return XmppChatbot::InternalError;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::messageReceived(const QXmppMessage& msg)
{
    if (msg.type() != QXmppMessage::Chat) {
        return;
    }
    if (!m_virtualUser) {
        return;
    }
    if (msg.body().isEmpty()) {
        return;
    }

    ContactInfo info = getContactInfo(getBareJid(msg.from()));

    // To reply a message we need some contact info such as full name
    // If we dont have the contact info, we enqueue the message and request the info
    // Once the info is available, we reply the queued message
    if (!info.isNull()) {
        replyMessage(msg, info);
    } else {
        requestContactInfo(msg);
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::ContactInfo Lvk::CA::XmppChatbot::getContactInfo(const QString &bareJid)
{
    QMutexLocker locker(m_contactInfoMutex);

    ContactInfo info;

    if (m_contactInfo.contains(bareJid)) {
        info = m_contactInfo[bareJid];
    }

    return info;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::requestContactInfo(const QXmppMessage &msg)
{
    QMutexLocker locker(m_messageQueueMutex);

    QString bareJid = getBareJid(msg.from());

    bool requested = false;

    for (int i = 0; i < m_messageQueue.size() && !requested; ++i) {
        if (getBareJid(m_messageQueue[i].from()) == bareJid) {
            requested = true;
        }
    }

    m_messageQueue.append(msg);

    if (!requested) {
        m_xmppClient->vCardManager().requestVCard(bareJid);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::vCardReceived(const QXmppVCardIq &vCard)
{
    QString bareJid = getBareJid(vCard.from());

    ContactInfo info;
    info.username = bareJid;
    info.fullname = vCard.fullName();

    {
        QMutexLocker locker(m_contactInfoMutex);
        m_contactInfo[bareJid] = info;
    }

    replyQueuedMessages(info);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::replyQueuedMessages(const ContactInfo &info)
{
    QMutexLocker locker(m_messageQueueMutex);

    const QString bareJid = info.username;

    for (int i = 0; i < m_messageQueue.size();) {
        if (getBareJid(m_messageQueue[i].from()) == bareJid) {
            replyMessage(m_messageQueue.takeAt(i), info);
        } else {
            ++i;
        }
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::replyMessage(const QXmppMessage &msg, const ContactInfo &info)
{
    QString from = msg.from();
    QString body = msg.body();

    QString response = m_virtualUser->getResponse(body, info);

    if (!response.isEmpty()) {
        m_xmppClient->sendPacket(QXmppMessage("", from, response));
    }
}
