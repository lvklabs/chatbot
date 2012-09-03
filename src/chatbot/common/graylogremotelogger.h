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

#ifndef LVK_CMN_GRAYLOGREMOTELOGGER_H
#define LVK_CMN_GRAYLOGREMOTELOGGER_H

#include "common/remotelogger.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Cmn
{

/// \ingroup Lvk
/// \addtogroup Cmn
/// @{

/**
 * \brief The GraylogRemoteLogger class logs messages in a Graylog Server.
 *
 * Graylog is an open source log management solution that stores logs in ElasticSearch database.
 *
 * The GraylogRemoteLogger sends messages to the server set in the configuration file.
 *
 * More info about Graylog at http://graylog2.org/
 */
class GraylogRemoteLogger : public RemoteLogger
{
public:

    /**
     * Graylog supported log formats.
     */
    enum LogFomat
    {
        GELF,       ///< Graylog Extended Log Format (always UDP connection)
        SyslogTCP,  ///< Syslog over TCP connection
        SyslogUDP   ///< Syslog over UDP connection
    };

    /**
     * Creates a GraylogRemoteLogger object.
     */
    GraylogRemoteLogger();

    /**
     * Creates a GraylogRemoteLogger object with \a format. If \a encrypt is true, messages
     * are encrypted before sending.
     */
    GraylogRemoteLogger(LogFomat format, bool encrypt);

    /**
     * \copydoc RemoteLogger::log(const QString&)
     */
    virtual int log(const QString &msg);

    /**
     * \copydoc RemoteLogger::log(const QString &, const FieldList&)
     */
    virtual int log(const QString &msg, const FieldList &fields);

private:

    LogFomat m_format;
    bool m_encrypt;
    QString m_host;
    unsigned m_udpPort;
    unsigned m_tcpPort;

    void initHostPort();
};

/// @}

} // namespace Cmn

/// @}

} // namespace Lvk


#endif // LVK_CMN_GRAYLOGREMOTELOGGER_H

