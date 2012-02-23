
#include "ZlibHelper.h"

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




