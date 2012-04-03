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

//#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::XmppChatbot(QObject *parent)
    : m_xmppClient(new QXmppClient(parent)),
      m_virtualUser(0),
      m_contactInfoMutex(new QMutex()),
      m_waitVCard(new QWaitCondition())
{
    connect(m_xmppClient, SIGNAL(messageReceived(const QXmppMessage&)),
            this, SLOT(messageReceived(const QXmppMessage&)));

    connect(&m_xmppClient->vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq&)),
            this, SLOT(vCardReceived(const QXmppVCardIq&)));

    connect(m_xmppClient, SIGNAL(connected()),    SIGNAL(connected()));
    connect(m_xmppClient, SIGNAL(disconnected()), SIGNAL(disconnected()));

    connect(m_xmppClient, SIGNAL(error(QXmppClient::Error)),
            SLOT(emitLocalError(QXmppClient::Error)));
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::XmppChatbot::~XmppChatbot()
{
    delete m_waitVCard;
    delete m_contactInfoMutex;
    delete m_virtualUser;
    delete m_xmppClient;
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::messageReceived(const QXmppMessage& message)
{
    if (message.type() != QXmppMessage::Chat) {
        return;
    }
    if (!m_virtualUser) {
        return;
    }
    if (message.body().isEmpty()) {
        return;
    }

    QString from = message.from();
    QString body = message.body();
    QString bareJid = from.split("/").at(0);

    // FIXME this blocks main thread! Use another thread
    //ContactInfo info = getContactInfo(bareJid);
    ContactInfo info;
    info.username = bareJid;

    QString response = m_virtualUser->getResponse(body, info);

    if (!response.isEmpty()) {
        m_xmppClient->sendPacket(QXmppMessage("", from, response));
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::connectToServer(const QString &user, const QString &passwd,
                                          const QString &host)
{
    m_xmppClient->connectToServer(user + "@" + host, passwd);
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

Lvk::CA::ContactInfo Lvk::CA::XmppChatbot::getContactInfo(const QString &bareJid)
{
    const int TIMEOUT_MS = 10*1000;

    QMutexLocker locker(m_contactInfoMutex);

    bool timeout = false;

    while (!m_contactInfo.contains(bareJid) && !timeout) {
        m_xmppClient->vCardManager().requestVCard(bareJid);

        if (!m_waitVCard->wait(m_contactInfoMutex, TIMEOUT_MS)) {
            timeout = true;
        }
    }

    if (!timeout) {
        return m_contactInfo[bareJid];
    } else {
        ContactInfo info;
        info.username = bareJid;

        return info;
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::XmppChatbot::vCardReceived(const QXmppVCardIq &vCard)
{
    QString bareJid = vCard.from().split("/").at(0);

    ContactInfo info;
    info.username = bareJid;
    info.fullname = vCard.fullName();

    QMutexLocker locker(m_contactInfoMutex);

    m_contactInfo[bareJid] = info;

    m_waitVCard->wakeAll();
}
