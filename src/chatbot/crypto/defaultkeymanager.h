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

#ifndef LVK_CRYPTO_DEFAULTKEYMANAGER_H
#define LVK_CRYPTO_DEFAULTKEYMANAGER_H

#include "crypto/keymanager.h"

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
 * \brief The DefaultKeyManager class provides the default implementation of the KeyManager
 *        interface
 */

class DefaultKeyManager : public KeyManager
{
public:

    /**
     * \copydoc KeyManager::getKey()
     */
    virtual QByteArray getKey(Role /*role*/)
    {
        return QByteArray(16, '\0');
    }

    /**
     * \copydoc KeyManager::getIV()
     */
    virtual QByteArray getIV(Role /*role*/)
    {
        return QByteArray(8, '\0');
    }
};

/// @}

} // namespace Crypto

/// @}

} // namespace Lvk


#endif // LVK_CRYPTO_DEFAULTKEYMANAGER_H

