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

#include "da-server/graylogremotelogger.h"
#include "da-server/gelf.h"
#include "da-server/syslog.h"
#include "common/version.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "crypto/cipher.h"
#include "crypto/keymanagerfactory.h"

#include <QUdpSocket>
#include <QTcpSocket>
#include <QDateTime>
#include <QDebug>
#include <memory>

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// convert RemoteLogger::FieldList to Gelf::FieldList
inline Lvk::DAS::Gelf::FieldList toGelfFields(const Lvk::DAS::RemoteLogger::FieldList &fields)
{
    Lvk::DAS::Gelf::FieldList gelfFields;
    foreach (const Lvk::DAS::RemoteLogger::Field &f, fields) {
        gelfFields.append(Lvk::DAS::Gelf::Field(f));
    }
    return gelfFields;
}

//--------------------------------------------------------------------------------------------------

// convert RemoteLogger::FieldList to QString
inline QString toString(const Lvk::DAS::RemoteLogger::FieldList &fields)
{
    QString s;
    foreach (const Lvk::DAS::RemoteLogger::Field &f, fields) {
        if (s.isEmpty()) {
            s += "{";
        } else {
            s += ",";
        }
        if (f.first.startsWith("_")) {
            s += QString("\"%1\":\"%2\"").arg(f.first, f.second);
        } else {
            s += QString("\"_%1\":\"%2\"").arg(f.first, f.second);
        }
    }

    if (!s.isEmpty()) {
        s += "}";
    }

    return s;
}

//--------------------------------------------------------------------------------------------------

inline int sendUdpMessage(const QByteArray &data, const QString &host, unsigned port)
{
    qint64 bytes = QUdpSocket().writeDatagram(data, QHostAddress(host), port);

    if (bytes == -1) {
        qWarning() << "sendUdpMessage error";
    }

    return bytes != -1 ? 0 : 1;
}

//--------------------------------------------------------------------------------------------------

inline int sendTcpMessage(const QByteArray &data, const QString &host, unsigned port)
{
    qint64 bytes = -1;

    try {
        QTcpSocket tcpSocket;

        qDebug() << "sendTcpMessage Host/port:" << host << port;

        tcpSocket.connectToHost(QHostAddress(host), port);

        if (!tcpSocket.waitForConnected()) {
            throw 1;
        }

        bytes = tcpSocket.write(data);

        if (bytes == -1) {
            throw 2;
        }

        if (!tcpSocket.waitForBytesWritten()) {
            throw 3;
        }

        qDebug() << "sendTcpMessage OK";

    } catch (int err) {
        qWarning() << "sendTcpMessage error" << err;
        bytes = -1;
    }

    return bytes != -1 ? 0 : 1;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// GraylogRemoteLogger
//--------------------------------------------------------------------------------------------------

Lvk::DAS::GraylogRemoteLogger::GraylogRemoteLogger()
    : m_format(GELF), m_udpPort(0), m_tcpPort(0)
{
    initHostPort();
}

//--------------------------------------------------------------------------------------------------

Lvk::DAS::GraylogRemoteLogger::GraylogRemoteLogger(LogFomat format)
    : m_format(format), m_udpPort(0), m_tcpPort(0)
{
    initHostPort();
}

//--------------------------------------------------------------------------------------------------

void Lvk::DAS::GraylogRemoteLogger::initHostPort()
{
    Cmn::Settings settings;
    m_host    = settings.value(SETTING_LOG_SERVER_HOST).toString();
    m_udpPort = settings.value(SETTING_LOG_SERVER_UDP_PORT).toUInt();
    m_tcpPort = settings.value(SETTING_LOG_SERVER_TCP_PORT).toUInt();
}

//--------------------------------------------------------------------------------------------------

int Lvk::DAS::GraylogRemoteLogger::log(const QString &msg)
{
    return log(msg, FieldList());
}

//--------------------------------------------------------------------------------------------------

int Lvk::DAS::GraylogRemoteLogger::log(const QString &msg, const FieldList &fields)
{
    QByteArray data;
    QString encMsg;

    // Build message

    switch (m_format) {
    case GELF:
        data = DAS::Gelf(Gelf::Informational, msg, toGelfFields(fields)).data();
        break;
    case SyslogTCP:
    case SyslogUDP:
        data = Syslog(msg + " " + toString(fields)).data();
        break;
    case EncSyslogTCP:
        if (encrypt(encMsg, msg + " " + toString(fields))) {
            data = Syslog(encMsg).data();
        }
        break;
   default:
        qCritical() << "GraylogRemoteLogger: Invalid log format" << m_format;
        break;
    }

    if (data.isNull()) {
        return 1;
    }

    // Send message with proper protocol

    switch (m_format) {
    case GELF:
    case SyslogUDP:
        return sendUdpMessage(data, m_host, m_udpPort);
    case SyslogTCP:
    case EncSyslogTCP:
        return sendTcpMessage(data, m_host, m_tcpPort);
    default:
        return 1;
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::DAS::GraylogRemoteLogger::encrypt(QString &cipherText, const QString &plainText)
{
    cipherText.clear();

    std::auto_ptr<Crypto::KeyManager> keyMgr(Crypto::KeyManagerFactory().create());

    QByteArray iv = keyMgr->getIV(Crypto::KeyManager::RemoteLoggerRole);
    QByteArray key = keyMgr->getKey(Crypto::KeyManager::RemoteLoggerRole);
    QByteArray data = plainText.toUtf8();

    if (Crypto::Cipher(iv, key).encrypt(data)) {
        cipherText = "::E::"; // prefix for encoded data
        cipherText += data.toBase64();
    } else {
        qCritical() << "GraylogRemoteLogger: Cannot encrypt data";
    }

    return !cipherText.isEmpty();
}

