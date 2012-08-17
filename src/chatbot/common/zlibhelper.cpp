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

#include "zlibhelper.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

int zlib_deflate(const char *source, long ssize, char *dest, long *dsize, int level)
{
    int ret, flush;
    z_stream strm;

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of buffer */
    do {
        strm.next_in = (Bytef *)source;

        if (ssize > CHUNK) {
            strm.avail_in = CHUNK;
            ssize -= CHUNK;
            source += CHUNK;
            flush = Z_NO_FLUSH;
        } else {
            strm.avail_in = ssize;
            ssize = 0;
            flush = Z_FINISH;
        }

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = (Bytef *)dest;
            ret = deflate(&strm, flush);    /* no bad return value */
            dest += (CHUNK - strm.avail_out);
        } while (strm.avail_out == 0);

        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);

    assert(ret == Z_STREAM_END);        /* stream will be complete */

    *dsize = strm.total_out;

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

}

//--------------------------------------------------------------------------------------------------
// ZLibHelper
//--------------------------------------------------------------------------------------------------

Lvk::Cmn::ZLibHelper::ZLibHelper()
{
}

//--------------------------------------------------------------------------------------------------

int Lvk::Cmn::ZLibHelper::deflate(const QByteArray &in, QByteArray &out)
{
    char *dest = new char[in.size()*2];
    long dsize = in.size()*2;

    int ret = zlib_deflate(in.data(), in.size(), dest, &dsize, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        out = QByteArray(dest, dsize);
    }

    delete[] dest;

    return ret;
}



