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

#ifndef LVK_CRYPTO_KEYMANAGER_H
#define LVK_CRYPTO_KEYMANAGER_H

#include <QByteArray>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Crypto
{

/// \ingroup Lvk
/// \addtogroup Crypto
/// @{

/**
 * \brief The KeyManager class provides access to the cryptographic keys required by the
 *        application
 */
class KeyManager
{
public:

    /**
     * Key Roles
     */
    enum Role
    {
        DefaultRole,
        LocalStatsRole,
        RemoteLoggerRole
    };

    /**
     * Destroys the object
     */
    virtual ~KeyManager() { }

    /**
     * Returns the key for the given \a role
     */
    virtual QByteArray getKey(Role role) = 0;

    /**
     * Returns the initialization vector for the given \a role
     */
    virtual QByteArray getIV(Role role) = 0;
};

/// @}

} // namespace Crypto

/// @}

} // namespace Lvk


#endif // LVK_CRYPTO_KEYMANAGER_H

