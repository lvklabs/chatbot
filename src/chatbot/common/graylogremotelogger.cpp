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

#include <QUdpSocket>
#include <QTcpSocket>
#include <QDateTime>
#include <QDebug>
#include <QHostInfo>

#define LOG_SERVER_HOST     "127.0.0.1" // TODO read from config file
#define LOG_SERVER_UDP_PORT 12201       // TODO read from config file
#define LOG_SERVER_TCP_PORT 10514       // TODO read from config file


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

inline int sendUdpMessage(const QByteArray &data)
{
    QHostAddress addr(LOG_SERVER_HOST);

    qint64 bytes = QUdpSocket().writeDatagram(data, addr, LOG_SERVER_UDP_PORT);

    if (bytes == -1) {
        qWarning() << "sendUdpMessage error";
    }

    return bytes != -1 ? 0 : 1;
}

//--------------------------------------------------------------------------------------------------

inline int sendTcpMessage(const QByteArray &data)
{
    qint64 bytes = -1;

    try {
        QTcpSocket tcpSocket;

        tcpSocket.connectToHost(QHostAddress(LOG_SERVER_HOST), LOG_SERVER_TCP_PORT);

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

        QByteArray priority = "<165>"; // FIXME
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
    : m_format(GELF)
{
}

//--------------------------------------------------------------------------------------------------

Lvk::Cmn::GraylogRemoteLogger::GraylogRemoteLogger(LogFomat format)
    : m_format(format)
{
}

//--------------------------------------------------------------------------------------------------

int Lvk::Cmn::GraylogRemoteLogger::log(const QString &msg)
{
    return log(msg, FieldList());
}

//--------------------------------------------------------------------------------------------------

int Lvk::Cmn::GraylogRemoteLogger::log(const QString &msg, const FieldList &fields)
{
    switch (m_format) {
    case GELF:
        return sendUdpMessage(Gelf(Gelf::Informational, msg, toGelfFields(fields)).data());
    case SyslogTCP:
        return sendTcpMessage(Syslog(msg + " " + toString(fields)).data());
    case SyslogUDP:
        return sendUdpMessage(Syslog(msg + " " + toString(fields)).data());
    default:
        return 1;
    }
}

