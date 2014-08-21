#include "lerror.h"
#include "lcross.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

void lerror_set_internal( lerror **err, const char *domain, const char *message ) {
    if ( err==NULL ) return;

    /** non si puo' inserire un errore su un altro e il messaggio deve sempre essere passato */
    l_assert( message!=NULL );
    l_assert( *err == NULL );

    *err = malloc( sizeof( struct lerror ) );
    (*err)->message = strdup( message );
    (*err)->stacktrace = lstring_new();

    if ( domain!=NULL ) {
	(*err)->domain = strdup( domain );
    } else {
	(*err)->domain = NULL;
    }
}

void lerror_delete( lerror **err ) {
    if ( err==NULL ) return;
    if ( *err==NULL ) return;

    free( (*err)->message );
    free( (*err)->domain );
    lstring_delete( (*err)->stacktrace );
    free( *err );
    *err = NULL;
}

void lerror_set_sprintf_internal( lerror **err, const char *domain, const char *message, ... ) {
    char buffer[2048];
    va_list args;

    l_assert( err==NULL || (*err)==NULL );
    l_assert( message!=NULL );

    va_start( args, message );
    vsnprintf( buffer, 2048, message, args );
    va_end( args );

    buffer[2047] = '0';
    lerror_set_internal( err, domain, buffer );
}

void lerror_add_prefix_internal( lerror **err, const char *prefix ) {
    char *buffer;

    l_assert( err!=NULL );
    l_assert( *err!=NULL );

    buffer = calloc( 1, strlen(prefix) + strlen((*err)->message) + 16 );
    strcpy( buffer, prefix );
    strcat( buffer, " - " );
    strcat( buffer, (*err)->message );

    free( (*err)->message );
    (*err)->message = buffer;
}

lbool lerror_propagate_internal( lerror **destination, lerror *source, const char *domain, const char *location ) {
    l_assert( destination==NULL || *destination==NULL );

    if ( source==NULL ) {
        return LFALSE;

    } else {
        lerror_add_stack_internal( &source, domain, location );
        
        if ( destination==NULL ) {
            free( source );
        } else {
            *destination = source;
        }

        return LTRUE;
    }
}

void lerror_add_stack_internal( lerror **destination, const char *domain, const char *location ) {
    l_assert( location!=NULL );
    l_assert( destination==NULL || *destination!=NULL );

    if ( destination!=NULL ) {
	if ( domain!=NULL ) {
	    lstring_append_cstr( (*destination)->stacktrace, domain );
	    lstring_append_cstr( (*destination)->stacktrace, " - " );
	}
	lstring_append_cstr( (*destination)->stacktrace, location );
	lstring_append_char( (*destination)->stacktrace, '\n' );
    }
}

void lerror_fill( lerror *error, lstring *str )    
{
    l_assert( error!=NULL );
    l_assert( str!=NULL );

    lstring_truncate( str, 0 );
    lstring_append_cstr( str, error->message );
    lstring_append_cstr( str, "\n--\n" );
    lstring_append_lstring( str, error->stacktrace );
}
