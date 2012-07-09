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

#include "gtalkchatbot.h"

#define GTALK_HOST    "gmail.com"

//--------------------------------------------------------------------------------------------------
// FbChatbot
//--------------------------------------------------------------------------------------------------

Lvk::CA::GTalkChatbot::GTalkChatbot(QObject *parent)
    : XmppChatbot(parent)
{
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::GTalkChatbot::connectToServer(const QString &user, const QString &passwd)
{
    return XmppChatbot::connectToServer(user.split("@").at(0), passwd, GTALK_HOST);
}

//--------------------------------------------------------------------------------------------------

void Lvk::CA::GTalkChatbot::connectToServer(const QString &user, const QString &passwd,
                                          const QString &host)
{
    return XmppChatbot::connectToServer(user, passwd, host);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::CA::GTalkChatbot::tlsRequired() const
{
    // FIXME information not exposed by QXmppClient:
    // m_xmppClient->serverFeatures().tlsMode() == QXmppStreamFeatures::Required
    // But Gtalk does require TLS
    return true;
}

