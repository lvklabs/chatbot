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

#ifndef LVK_CA_VIRTUALUSER_H
#define LVK_CA_VIRTUALUSER_H

#include <QString>
#include <QPixmap>

#include "chat-adapter/contactinfo.h"
#include "common/conversation.h"

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
 * \brief The VirtualUser class provides information to the chatbot to behave like a human being.
 *
 * At least VirtualUser class shall provide two methods: one to get a response and another one
 * to show a custom avatar.
 */
class VirtualUser
{
public:

    /**
     * Destroys the object
     */
    virtual ~VirtualUser() {}

    /**
     * Returns an entry with the response for the given \a input and \a contact.
     * If no response is found, it returns a null entry.
     */
    virtual Cmn::Conversation::Entry getEntry(const QString &input,
                                              const CA::ContactInfo &contact) = 0;

    /**
     * Returns the avatar of the virtual user.
     */
    virtual QPixmap getAvatar() = 0;
};

/// @}

} // namespace CA

/// @}

} // namespace Lvk


#endif // LVK_CA_VIRTUALUSER_H
