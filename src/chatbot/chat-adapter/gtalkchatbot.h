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

#ifndef LVK_CA_GTALKCHATBOT_H
#define LVK_CA_GTALKCHATBOT_H

#include "chat-adapter/xmppchatbot.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace CA
{

/// \ingroup Lvk
/// \addtogroup CA
/// @{

/**
 * \brief The GTalkChatbot class provides a chatbot for Google GTalk.
 *
 * GTalk is the chat service of Google. GTalkChatbot works with the GTalk client, GMail chat and
 * any other Google service that uses chat.
 *
 * GTalkChatbot is a very thin layer over the Xmpp chatbot because GTalk chat is XMPP compliant
 *
 * \see XmppChatbot
 */

class GTalkChatbot : public XmppChatbot
{
public:

    /**
     * Constructs a GTalk chatbot with parent object \a parent
     */
    GTalkChatbot(QObject *parent = 0);

    /**
     * Connects to the GTalk server using \a user and \a password.
     * Emits \a connected signal on success. Otherwise; emits \a connectionError.
     * If the connection ends prematurely, emits \a disconnected.
     */
    virtual void connectToServer(const QString &user, const QString &passwd);

protected:
    virtual bool tlsRequired() const;

private:
    virtual void connectToServer(const QString &user, const QString &passwd, const QString &host);
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk

#endif // LVK_CA_GTALKCHATBOT_H
