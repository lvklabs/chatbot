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

#include <QUdpSocket>

#define LOG_SERVER_HOST "127.0.0.1" // TODO read from config file
#define LOG_SERVER_PORT 12201       // TODO read from config file

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// Send gelf message to remote server

inline int sendMessage(const Lvk::Cmn::Gelf &msg)
{
    if (msg.data().size() == 0) {
        return false;
    }

    QHostAddress addr(LOG_SERVER_HOST);

    qint64 bytes = QUdpSocket().writeDatagram(msg.data(), addr, LOG_SERVER_PORT);

    if (bytes == -1) {
        //std::cout << "ERROR: UDP write failed" << std::endl;
    }

    return bytes != -1 ? 0 : 1;
}

//--------------------------------------------------------------------------------------------------
// convert RemoteLogger::FieldList to Gelf::FieldList

inline Lvk::Cmn::Gelf::FieldList convert(Lvk::Cmn::RemoteLogger::FieldList fields)
{
    Lvk::Cmn::Gelf::FieldList gelfFields;
    foreach (const Lvk::Cmn::RemoteLogger::Field &f, fields) {
        gelfFields.append(Lvk::Cmn::Gelf::Field(f));
    }
    return gelfFields;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// GraylogRemoteLogger
//--------------------------------------------------------------------------------------------------

Lvk::Cmn::GraylogRemoteLogger::GraylogRemoteLogger()
{
}

//--------------------------------------------------------------------------------------------------

int Lvk::Cmn::GraylogRemoteLogger::log(const QString &msg)
{
    return sendMessage(Gelf(Gelf::Informational, msg));
}

//--------------------------------------------------------------------------------------------------

int Lvk::Cmn::GraylogRemoteLogger::log(const QString &msg, const FieldList &fields)
{
    return sendMessage(Gelf(Gelf::Informational, msg, convert(fields)));
}

