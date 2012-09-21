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

#ifndef LVK_DAS_ZLIBHELPER_H
#define LVK_DAS_ZLIBHELPER_H

#include <zlib.h>
#include <QByteArray>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace DAS
{

/// \ingroup Lvk
/// \addtogroup DAS
/// @{

/**
 * \brief The ZLibHelper class provides a wrapper to the Zlib library to compress
 *        data easily
 */
class ZLibHelper
{
public:
    /**
     * Compress (deflates) \a source and stores the result in \a dest.
     * Returns Z_OK on success, Z_MEM_ERROR if memory could not be allocated for processing,
     * Z_STREAM_ERROR if an invalid compression level is supplied, Z_VERSION_ERROR if the version
     * of zlib.h and the version of the library linked do not match, or Z_ERRNO if there is
     * an error reading or writing the files.
    */
    static int deflate(const QByteArray &source, QByteArray &dest);

private:
    ZLibHelper();
};

/// @}

} // namespace DAS

/// @}

} // namespace Lvk


#endif // LVK_DAS_ZLIBHELPER_H

