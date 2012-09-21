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

#include "syslog.h"
#include "common/version.h"

#include <QString>
#include <QHostInfo>
#include <QDateTime>
#include <QDebug>

#define NILVALUE            "-"
#define SP                  " "
#define SYSLOG_VERSION      "1"
#define SYSLOG_DATE_FORMAT  "yyyy-MM-ddThh:mm:ss.zzz-03:00"

//--------------------------------------------------------------------------------------------------
// Syslog
//--------------------------------------------------------------------------------------------------

Lvk::DAS::Syslog::Syslog(const QString &msg)
{
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

    //qDebug() << "Syslog message:" << m_data;
}

//--------------------------------------------------------------------------------------------------

const QByteArray & Lvk::DAS::Syslog::data() const
{
    return m_data;
}

