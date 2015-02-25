/*
About: License

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

Author: Leonardo Cecchi <mailto:leonardoce@interfree.it>
*/ 

#include "sqlhelper.h"
#include "lstring.h"
#include "lmemory.h"
#include <stdlib.h>

struct SqlHelper {
    lstring* query;
    lstring* buffer;
    int quantiCampi;
};

SqlHelper *SqlHelper_init( const char *nomeTabella ) {
	SqlHelper *self = (SqlHelper *)lmalloc( sizeof(SqlHelper) );

    self->query = lstring_new_from_cstr( "INSERT INTO " );
    self->query = lstring_append_cstr_f( self->query, nomeTabella );
    self->query = lstring_append_cstr_f( self->query, " (" );

    self->quantiCampi = 0;

    self->buffer = lstring_new();

    return self;
}

void SqlHelper_aggiungi_campo( SqlHelper *self, const char *nomeCampo ) {
    if ( !lstring_ends_with_cstr( self->query, "," ) && !lstring_ends_with_cstr( self->query, "(" ) ) {
        self->query = lstring_append_cstr_f( self->query, "," );
    }
    self->query = lstring_append_cstr_f( self->query, nomeCampo );

    self->quantiCampi = self->quantiCampi + 1;
}

const char *SqlHelper_to_insert( SqlHelper *self ) {
    int i;

    self->buffer = lstring_from_cstr_f( self->buffer, self->query );
    self->buffer = lstring_append_cstr_f( self->buffer, ") VALUES (" );

    for ( i=0; i<self->quantiCampi; i++ ) {
        if ( i!=0 ) {
            self->buffer = lstring_append_cstr_f( self->buffer, "," );
        }
        self->buffer = lstring_append_cstr_f( self->buffer, "?" );
    }

    self->buffer = lstring_append_cstr_f( self->buffer, ")" );
    return self->buffer;
}

void SqlHelper_destroy( SqlHelper *self ) {
    if ( !self ) return;
    lstring_delete( self->query );
    lstring_delete( self->buffer );
    lfree( self );
}

