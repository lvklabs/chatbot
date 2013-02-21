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

#include "testxmppclient.h"
#include "QXmppMessage.h"
#include <QApplication>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QTest>

//--------------------------------------------------------------------------------------------------
// TestXmppClient
//--------------------------------------------------------------------------------------------------

TestXmppClient::TestXmppClient(QObject *parent /*= 0*/)
    : QObject(parent),
      m_xmppClient(new QXmppClient()),
      m_mutex(new QMutex()),
      m_condWait(new QWaitCondition()),
      m_status(Disconnected)
{

    connect(m_xmppClient, SIGNAL(messageReceived(const QXmppMessage&)),
            SLOT(onMessageReceived(const QXmppMessage&)));

    connect(m_xmppClient, SIGNAL(connected()), SLOT(onConnectionOk()));

    connect(m_xmppClient, SIGNAL(error(QXmppClient::Error)),
            SLOT(onConnectionError(QXmppClient::Error)));

    connect(m_xmppClient, SIGNAL(disconnected()), SLOT(onDisconnection()));
}

//--------------------------------------------------------------------------------------------------

TestXmppClient::~TestXmppClient()
{
    delete m_xmppClient;
}

//--------------------------------------------------------------------------------------------------

bool TestXmppClient::connectClient(const QString &jid, const QString &passwd)
{
    m_jid = jid;

    m_status = Connecting;

    m_xmppClient->connectToServer(jid, passwd);

    int retries = 0;
    const int MAX_RETRIES = 60;

    while (m_status == Connecting && retries < MAX_RETRIES) {

        QTest::qSleep(500);

        //http://stackoverflow.com/questions/6433933/qtcpclient-successfully-connects-but-not-to-my-
        //server-where-is-it-connecting
        qApp->processEvents();

        retries++;
    }

    return isConnected();
}

//--------------------------------------------------------------------------------------------------

void TestXmppClient::disconnectClient()
{
    m_status = Disconnected;

    m_xmppClient->disconnectFromServer();
}

//--------------------------------------------------------------------------------------------------

bool TestXmppClient::isConnected()
{
    return m_status == Connected;
}

//--------------------------------------------------------------------------------------------------

void TestXmppClient::sendMessage(const QString &to, const QString &msg)
{
    if (!isConnected()) {
        return;
    }

    {
        QMutexLocker locker(m_mutex);
        m_response = QString();
    }

    m_xmppClient->sendPacket(QXmppMessage("", to, msg));
}

//--------------------------------------------------------------------------------------------------

bool TestXmppClient::waitForResponse(QString &response, int msecs)
{
    if (!isConnected()) {
        return false;
    }

    if (m_response.isNull()) {
        int time = 0;
        const int TICK = 500;

        while (time < msecs) {
            QTest::qSleep(TICK);
            time += TICK;

            {
                QMutexLocker locker(m_mutex);
                if (!m_response.isNull()) {
                    response = m_response;
                    break;
                }
            }

            //http://stackoverflow.com/questions/6433933/qtcpclient-successfully-connects-but-not-
            //to-my-server-where-is-it-connecting
            qApp->processEvents();
        }
    }

//    if (m_response.isNull()) {
//
//        QMutexLocker locker(m_mutex);
//        m_condWait->wait(m_mutex, msecs);
//    }
//    response = m_response;

    return !response.isNull();
}

//--------------------------------------------------------------------------------------------------

void TestXmppClient::onConnectionOk()
{
    m_status = Connected;
}

//--------------------------------------------------------------------------------------------------

void TestXmppClient::onConnectionError(QXmppClient::Error)
{
    m_status = Disconnected;
}

//--------------------------------------------------------------------------------------------------

void TestXmppClient::onDisconnection()
{
    m_status = Disconnected;
}

//--------------------------------------------------------------------------------------------------

void TestXmppClient::onMessageReceived(const QXmppMessage &msg)
{
    {
        QMutexLocker locker(m_mutex);

        m_response = msg.body();

        if (m_response.isNull()) {
            m_response = QString("");
        }
    }

    //m_condWait->wakeAll();
}


