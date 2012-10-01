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

#include "back-end/chatbotfactory.h"
#include "chat-adapter/fbchatbot.h"
#include "chat-adapter/gtalkchatbot.h"

//--------------------------------------------------------------------------------------------------
// ChatbotFactory
//--------------------------------------------------------------------------------------------------

Lvk::CA::Chatbot * Lvk::BE::ChatbotFactory::createChatbot(Lvk::BE::ChatType type)
{
    return createChatbot("", type);
}

//--------------------------------------------------------------------------------------------------

Lvk::CA::Chatbot * Lvk::BE::ChatbotFactory::createChatbot(const QString &id, Lvk::BE::ChatType type)
{
    switch (type) {
    case Lvk::BE::FbChat:
        return new Lvk::CA::FbChatbot(id);
    case Lvk::BE::GTalkChat:
        return new Lvk::CA::GTalkChatbot(id);
    default:
        qCritical() << "createChatbot() Invalid chat type" << type;
        return 0;
    }
}

