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

#ifndef LVK_TESTXMPPCLIENT_H
#define LVK_TESTXMPPCLIENT_H

#include <QString>
#include <QObject>
#include "QXmppClient.h"

class QMutex;
class QWaitCondition;

class TestXmppClient : public QObject
{
    Q_OBJECT

public:
    TestXmppClient(QObject *parent = 0);

    ~TestXmppClient();

    bool connectClient(const QString &jid, const QString &passwd);

    void disconnectClient();

    bool isConnected();

    void sendMessage(const QString &to, const QString &msg);

    bool waitForResponse(QString &response, int msecs);

private slots:
    void onConnectionOk();

    void onConnectionError(QXmppClient::Error);

    void onDisconnection();

    void onMessageReceived(const QXmppMessage&);

private:
    QString m_jid;
    QString m_response;
    QXmppClient *m_xmppClient;
    QMutex *m_mutex;
    QWaitCondition *m_condWait;
    enum { Disconnected, Connecting, Connected } m_status;
};

#endif // LVK_TESTXMPPCLIENT_H
