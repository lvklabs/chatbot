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

#ifndef LVK_BE_CHATBOTFACTORY_H
#define LVK_BE_CHATBOTFACTORY_H

#include "chat-adapter/chatbot.h"
#include "back-end/chattype.h"

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The ChatbotFactory class provides a factory of chatbots
 */

class ChatbotFactory
{
public:

    /**
     * Creates a chatbot of \a type with null id
     */
    CA::Chatbot *createChatbot(Lvk::BE::ChatType type);

    /**
     * Creates a chatbot of \a type with \a id.
     */
    CA::Chatbot *createChatbot(const QString &id, Lvk::BE::ChatType type);

};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_CHATBOTFACTORY_H

