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

#include "cipher.h"
#include <QByteArray>
#include <QDebug>

#ifdef OPENSSL_SUPPORT
# include <openssl/evp.h>
# include <openssl/err.h>
#endif


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

#ifdef OPENSSL_SUPPORT

//--------------------------------------------------------------------------------------------------
// Blowfish CBC encryption

inline bool bf_encrypt(QByteArray &data, const QByteArray &iv_, const QByteArray &key_)
{
    if (key_.size() != EVP_CIPHER_key_length(EVP_bf_cbc())) {
        qCritical() << "bf_encrypt invalid key size";
        return false;
    }

    const unsigned char *iv = reinterpret_cast<const unsigned char*>(iv_.constData());
    const unsigned char *key = reinterpret_cast<const unsigned char*>(key_.constData());
    const unsigned char *inbuf = reinterpret_cast<const unsigned char*>(data.constData());
    int inlen = data.size();

    // TODO initialize context only once
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, EVP_bf_cbc(), NULL, key, iv);

    bool success = false;
    unsigned char *outbuf = new unsigned char[inlen + EVP_CIPHER_CTX_block_size(&ctx)];
    int outlen = 0;

    // TODO encrypt using a buffer of a fixed size to avoid running out of memory with very big data
    if (EVP_EncryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) {
        int tmplen = 0;
        if (EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen)) {
            outlen += tmplen;
            data = QByteArray(reinterpret_cast<const char*>(outbuf), outlen);
            success = true;
        }
    }

    if (!success) {
        qCritical() << "bf_encrypt failed!";
        //ERR_print_errors_fp(stderr);
    }

    EVP_CIPHER_CTX_cleanup(&ctx);
    delete[] outbuf;

    return success;
}

//--------------------------------------------------------------------------------------------------
// Blowfish CBC decryption
inline bool bf_decrypt(QByteArray &data, const QByteArray &iv_, const QByteArray &key_)
{
    if (key_.size() != EVP_CIPHER_key_length(EVP_bf_cbc())) {
        qCritical() << "bf_decrypt invalid key size";
        return false;
    }

    const unsigned char *iv = reinterpret_cast<const unsigned char*>(iv_.constData());
    const unsigned char *key = reinterpret_cast<const unsigned char*>(key_.constData());
    const unsigned char *inbuf = reinterpret_cast<const unsigned char*>(data.constData());
    int inlen = data.size();

    // TODO initialize context only once
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_DecryptInit_ex(&ctx, EVP_bf_cbc(), NULL, key, iv);

    bool success = false;
    unsigned char *outbuf = new unsigned char[inlen];
    int outlen = 0;

    // TODO decrypt using a buffer of a fixed size to avoid running out of memory with very big data
    if (EVP_DecryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) {
        int tmplen = 0;
        if (EVP_DecryptFinal_ex(&ctx, outbuf + outlen, &tmplen)) {
            outlen += tmplen;
            success = true;
            data = QByteArray(reinterpret_cast<const char*>(outbuf), outlen);
        }
    }

    if (!success) {
        qCritical() << "bf_decrypt failed!";
        //ERR_print_errors_fp(stderr);
    }

    EVP_CIPHER_CTX_cleanup(&ctx);
    delete[] outbuf;

    return success;
}

#endif // OPENSSL_SUPPORT

//--------------------------------------------------------------------------------------------------

inline bool null_encrypt(QByteArray &/*data*/, const QByteArray &/*key*/)
{
    return true;
}

//--------------------------------------------------------------------------------------------------

inline bool null_decrypt(QByteArray &/*data*/, const QByteArray &/*key*/)
{
    return true;
}

}

//--------------------------------------------------------------------------------------------------
// Cipher
//--------------------------------------------------------------------------------------------------

Lvk::Crypto::Cipher::Cipher(const QByteArray &iv, const QByteArray &key)
    : m_iv(iv), m_key(key)
{
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Crypto::Cipher::encrypt(QByteArray &data)
{
#ifdef OPENSSL_SUPPORT
    return bf_encrypt(data, m_iv, m_key);
#else
    return null_encrypt(data, m_key);
#endif
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Crypto::Cipher::decrypt(QByteArray &data)
{
#ifdef OPENSSL_SUPPORT
    return bf_decrypt(data, m_iv, m_key);
#else
    return null_decrypt(data, m_key);
#endif
}


