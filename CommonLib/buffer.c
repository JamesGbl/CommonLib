/*
Author: Leonardo Cecchi <leonardoce@interfree.it>

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/ 
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "buffer.h"
#include "lcross.h"
#include "lmemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemBuffer {
    char *buf;
    int space;
    int len;
};

MemBuffer* MemBuffer_new( int reservedSpace ) {
    MemBuffer *self = (MemBuffer *) lmalloc(sizeof(MemBuffer));

    self->buf = lmalloc( reservedSpace );
    self->space = reservedSpace;
    self->len = 0;

    return self;
}

int MemBuffer_len( MemBuffer* self ) {
    return self->len;
}

char* MemBuffer_address( MemBuffer* self ) {
    return self->buf;
}

int MemBuffer_reserve( MemBuffer* self ) {
    return self->space;
}

void MemBuffer_write( MemBuffer* self, void *addr, size_t size ) {
    int postLen;

    postLen = self->len + size;
    if ( postLen > self->space ) {
        self->space = 2<<l_log2(postLen + 128);
        self->buf = lrealloc( self->buf, self->space );
    } 

    memcpy( self->buf + self->len, addr, size );
    self->len = postLen;
}

void MemBuffer_write_char( MemBuffer* self, char c ) {
    int postLen;

    postLen = self->len + 1;
    if ( postLen > self->space ) {
        self->space = 2<<l_log2(postLen + 128);
        self->buf = lrealloc( self->buf, self->space );
    } 

    self->buf[self->len] = c;
    self->len = postLen;
}

void MemBuffer_setlen( MemBuffer* self, int len ) {
    if ( self->space < len ) {
        self->len = self->space;
    } else {
        self->len = len;
    }
}

void MemBuffer_destroy( MemBuffer* self ) {
    if ( !self ) {
        return;
    }

    lfree( self->buf );
    lfree( self );
}

MemBuffer* MemBuffer_new_fromfile( const char *fileName ) {
    MemBuffer *buffer;
    const int tmpSize = 1024;
    char tmp[1024];
    int byteRead;
    FILE *in;

    in = fopen( fileName, "rb" );
    if ( !in ) {
        return NULL;
    }

    buffer = MemBuffer_new( 4096 );
    while ( !feof(in) ) {
        byteRead = fread( tmp, 1, tmpSize, in );
        if ( byteRead<=0 ) {
            break;
        }

        MemBuffer_write( buffer, tmp, byteRead );
    }
    fclose( in );

    return buffer;
}
