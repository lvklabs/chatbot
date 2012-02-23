#ifndef ZLIBHELPER_H
#define ZLIBHELPER_H

#include "zlib.h"

/* Compress source and stores the result in dest
   Returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int zlib_deflate(const char *source, long ssize, char *dest, long *dsize, int level);

#endif // ZLIBHELPER_H
