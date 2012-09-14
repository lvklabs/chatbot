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

#include "common/graylogremotelogger.h"
#include "common/gelf.h"
#include "common/version.h"
#include "common/settings.h"
#include "common/settingskeys.h"
#include "common/cipher.h"

#include <QUdpSocket>
#include <QTcpSocket>
#include <QDateTime>
#include <QDebug>
#include <QHostInfo>

#ifndef RLOG_CRYPTO_KEY
#define RLOG_CRYPTO_KEY   { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
#endif

static const char s_rlogCryptoKey[] = RLOG_CRYPTO_KEY;

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// convert RemoteLogger::FieldList to Gelf::FieldList
inline Lvk::Cmn::Gelf::FieldList toGelfFields(const Lvk::Cmn::RemoteLogger::FieldList &fields)
{
    Lvk::Cmn::Gelf::FieldList gelfFields;
    foreach (const Lvk::Cmn::RemoteLogger::Field &f, fields) {
        gelfFields.append(Lvk::Cmn::Gelf::Field(f));
    }
    return gelfFields;
}

//--------------------------------------------------------------------------------------------------

// convert RemoteLogger::FieldList to QString
inline QString toString(const Lvk::Cmn::RemoteLogger::FieldList &fields)
{
    QString s;
    foreach (const Lvk::Cmn::RemoteLogger::Field &f, fields) {
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

//--------------------------------------------------------------------------------------------------

class Syslog
{
public:

    Syslog(const QString &msg)
    {
        #define NILVALUE            "-"
        #define SP                  " "
        #define SYSLOG_VERSION      "1"
        #define SYSLOG_DATE_FORMAT  "yyyy-MM-ddThh:mm:ss.zzz+03:00" // FIXME timezone

        QByteArray priority = "<165>"; // FIXME priority
        QByteArray date = QDateTime::currentDateTime().toString(SYSLOG_DATE_FORMAT).toAscii();
        QByteArray host = QHostInfo::localHostName().toAscii();
        QByteArray app = APP_NAME "_" APP_VERSION_STR;

        m_data.append(priority);              // PRI
        m_data.append(SYSLOG_VERSION);        // VERSION
        m_data.append(SP);                    // SP
        m_data.append(date);                  // DATETIME
        m_data.append(SP);                    // SP
        m_data.append(host);                  // HOSTNAME
        m_data.append(SP);                    // SP
        m_data.append(app);                   // APP-NAME
        m_data.append(SP);                    // SP
        m_data.append(NILVALUE);              // PROCID
        //data.append(SP);                    // SP
        //data.append(NILVALUE);              // MSGID
        //data.append(SP);                    // SP
        //data.append(NILVALUE);              // STRUCTURED-DATA
        m_data.append(SP);                    // SP
        m_data.append(msg.toUtf8());          // MSG

        qDebug() << "Syslog message:" << m_data;
    }

    const QByteArray& data() const
    {
        return m_data;
    }

private:
    QByteArray m_data;
};


} // namespace


//--------------------------------------------------------------------------------------------------
// GraylogRemoteLogger
//--------------------------------------------------------------------------------------------------

Lvk::Cmn::GraylogRemoteLogger::GraylogRemoteLogger()
    : m_format(GELF), m_udpPort(0), m_tcpPort(0)
{
    initHostPort();
}

//--------------------------------------------------------------------------------------------------

Lvk::Cmn::GraylogRemoteLogger::GraylogRemoteLogger(LogFomat format)
    : m_format(format), m_udpPort(0), m_tcpPort(0)
{
    initHostPort();
}

//--------------------------------------------------------------------------------------------------

void Lvk::Cmn::GraylogRemoteLogger::initHostPort()
{
    Cmn::Settings settings;
    m_host    = settings.value(SETTING_LOG_SERVER_HOST).toString();
    m_udpPort = settings.value(SETTING_LOG_SERVER_UDP_PORT).toUInt();
    m_tcpPort = settings.value(SETTING_LOG_SERVER_TCP_PORT).toUInt();
}

//--------------------------------------------------------------------------------------------------

int Lvk::Cmn::GraylogRemoteLogger::log(const QString &msg)
{
    return log(msg, FieldList());
}

//--------------------------------------------------------------------------------------------------

int Lvk::Cmn::GraylogRemoteLogger::log(const QString &msg, const FieldList &fields)
{
    QByteArray data;
    QString encMsg;

    // Build message

    switch (m_format) {
    case GELF:
        data = Cmn::Gelf(Gelf::Informational, msg, toGelfFields(fields)).data();
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

bool Lvk::Cmn::GraylogRemoteLogger::encrypt(QString &cipherText, const QString &plainText)
{
    cipherText.clear();

    QByteArray data = plainText.toUtf8();
    QByteArray key(s_rlogCryptoKey, sizeof(s_rlogCryptoKey));

    if (Cmn::Cipher().encrypt(data, key)) {
        cipherText = "::E::"; // prefix for encoded data
        cipherText += data.toBase64();
    } else {
        qCritical() << "GraylogRemoteLogger: Cannot encrypt data";
    }

    return !cipherText.isEmpty();
}

