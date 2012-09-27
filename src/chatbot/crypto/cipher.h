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

#ifndef LVK_CRYPTO_CIPHER_H
#define LVK_CRYPTO_CIPHER_H

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

// TODO define Cipher as abstract interface

/**
 * \brief The Cipher class provides a symmetric block cipher to encrypt or decrypt data.
 */
class Cipher
{
public:

    /**
     * Constructs a Cipher with initialization vector \a iv  and key \a key
     */
    Cipher(const QByteArray &iv, const QByteArray &key);

    /**
     * Encrypts \a data using the given \a key. Returns true on success; Otherwise false.
     */
    bool encrypt(QByteArray &data);

    /**
     * Decrypts \a data using the given \a key. Returns true on success; Otherwise false.
     */
    bool decrypt(QByteArray &data);

private:
    QByteArray m_iv;
    QByteArray m_key;
};

/// @}

} // namespace Crypto

/// @}

} // namespace Lvk


#endif // LVK_CRYPTO_CIPHER_H

