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

#include "lstring.h"
#include "lcross.h"
#include "lmemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

static void lstring_initialize( lstring* str )
{
    str->buf = NULL;
    str->bufLen = 0;
    str->len = 0;
}

static void lstring_finalize( lstring* str )
{
	if ( str==NULL ) return;

    if( str->buf!=NULL ) 
    {
        lfree(str->buf);
    }
    lstring_initialize( str );
}

/*
** Costruisce una nuova stringa vuota
*/
lstring* lstring_new( void )
{
    lstring* str = (lstring *)lmalloc(sizeof(struct lstring));
    lstring_initialize(str);
    return str;
}

/*
** Costruisce una nuova stringa data la relativa
** stringa C
*/
lstring* lstring_new_from_cstr( const char *cstr )
{
    lstring* str = lstring_new();
    lstring_from_cstr(str, cstr);
    return str;
}    

/*
** Costruisce una nuova stringa prendendone
** un'altra nella stessa struttura
*/
lstring* lstring_new_from_lstr( lstring* self )
{
    lstring* str = (lstring *)lmalloc(sizeof(struct lstring));
    str->bufLen = self->bufLen;
    str->len = self->len;

    str->buf = (char *)lmalloc(self->bufLen);
    memcpy( str->buf, self->buf, self->len+1 );
    
    return str;
}

/*
** Reinizializza una lstring a partire da una stringa
** C
*/
void lstring_from_cstr( lstring* str, const char *other )
{
    lstring_truncate( str, 0 );
    if ( other!=NULL ) {
        lstring_append_generic( str, other, strlen(other) );
    }
}

/*
** Reinizializza una lstring a partire da una lstring
*/
void lstring_from_lstr( lstring* str, lstring *other )
{
    l_assert( str!=NULL );
    l_assert( other!=NULL );

    lstring_truncate( str, 0 );
    if ( other->buf!=NULL ) {
        lstring_append_generic( str, other->buf, other->len );
    }
}

/*
** Dealloca una stringa. Dopo questa chiamata la stringa
** non e' piu' valida
*/
void lstring_delete( lstring* str )
{
    if ( str==NULL ) return;

    lstring_finalize(str);
    lfree(str);
}    

void lstring_append_generic( lstring* str, const char *other, int otherLen)
{
    int newLen = otherLen + str->len;
    int newBufLen = 2<<l_log2(newLen + 50);

    if( str->bufLen > newBufLen )
    {
        newBufLen = str->bufLen;
    }    

    if( str->buf==NULL )
    {
        str->buf = lmalloc( newBufLen );
    } else if( str->bufLen<newBufLen )
    {
        str->buf = lrealloc(str->buf, newBufLen);
    }    

    memcpy( str->buf+str->len, other, otherLen );
    str->buf[str->len+otherLen]=0;
    str->len = newLen;
    str->bufLen = newBufLen;
}

/*
** Aggiunge, a una stringa, una stringa C.
** \param str Stringa alla quale aggiungere
** \param other Stringa da aggiungere
*/
void lstring_append_cstr( lstring* str, const char *other )
{
    int otherLen;

    l_assert( str!=NULL );
    l_assert( other!=NULL );

    otherLen = strlen(other);
    lstring_append_generic( str, other, otherLen );
}

/*
** Aggiunge un carattere
*/
void lstring_append_char( lstring* str, char c ) {
    l_assert( str!=NULL );

    lstring_append_generic( str, &c, 1 );
}

/*
** Aggiunge, a una stringa, una stringa.
** \param str Stringa alla quale aggiungere
** \param other Stringa da aggiungere
*/
void lstring_append_lstring( lstring* str, lstring* other )
{
    l_assert( str!=NULL );
    l_assert( other!=NULL );
    lstring_append_generic( str, other->buf, other->len );
}    

/*
** Converte una stringa in una stringa C
*/
const char *lstring_to_cstr( lstring* str )
{
    l_assert( str!=NULL );
    return str->buf;
}    

/*
** Prepara il buffer di una stringa a disporre
** di almeno un certo numero di caratteri
** \param str La stringa da preparare
** \param len Il numero di caratteri
*/
void lstring_reserve( lstring* str, int len )
{
    l_assert( str!=NULL );
    if( str->bufLen<len )
    {
        str->buf = lrealloc( str->buf, sizeof(char)*len );
        str->bufLen = len;
    }
}    

/*
** Pulisce tutti gli spazi all'inizio di una stringa
** \param str Stringa da pulire
*/
void lstring_ltrim( lstring* str )
{
    int i;

    l_assert( str!=NULL );

    if( str->buf!=NULL && str->len>0 )
    {
        i = 0;
        while( str->buf[i] != 0 && isspace(str->buf[i]) )
        {
            i++;
        }

        if( i>0 )
        {    
            str->len = str->len - i;
            memmove( str->buf, str->buf+i, str->len );
            str->buf[str->len] = 0;
        }
    }    
}

/*
** Pulisce tutti gli spazi alla fine di una stringa
** \param str La stringa da pulire
*/
void lstring_rtrim( lstring* str )
{
    l_assert( str!=NULL );

    if( str->buf!=NULL && str->len>0 )
    {
        while( str->len > 0 && isspace(str->buf[str->len-1]) )
        {
            str->len = str->len-1;
        }    
        str->buf[str->len] = 0;
    }
}

/*
** Pulisce tutti gli spazi all'inizio e alla fine di una stringa
** \param str La stringa da pulire
*/
void lstring_trim( lstring* str )
{
    l_assert( str!=NULL );

    lstring_rtrim( str );
    lstring_ltrim( str );
}    

/*
** Controlla se una stringa ne contiene un'altra
*/
int lstring_contains( lstring* str, const char *other )
{
    l_assert( str!=NULL );
    l_assert( other!=NULL );

    if( str->buf==NULL )
    {
        return 0;
    }    else if( strstr( str->buf, other )!=NULL )
    {
        return 1;
    } else
    {
        return 0;
    }    
}

/*
** Controlla l'uguaglianza di due stringhe, di cui una in stile C
*/
int lstring_equals_cstr( lstring* str, const char *cstr )
{
    l_assert( str!=NULL );
    l_assert( cstr!=NULL );

    if( str->buf==NULL )
    {
        return cstr==NULL;
    } else if( 0==strcmp(str->buf, cstr) )
    {
        return 1;
    } else
    {
        return 0;
    }
}    

/*
** Controlla l'uguaglianza di due stringhe
*/
int lstring_equals_lstr( lstring* str_1, lstring* str_2 )
{
    l_assert( str_1!=NULL );
    l_assert( str_2!=NULL );

    if( str_1->buf==NULL )
    {
        return str_2->buf==NULL;
    } else if( str_2->buf==NULL )
    {
        return 0;
    } else if( str_1->len != str_2->len )
    {
        return 0;
    } else if( str_1->buf==str_2->buf )
    {
        return 1;
    } else if( 0==memcmp(str_1->buf, str_2->buf, str_1->len) )
    {
        return 1;
    } else
    {
        return 0;
    }
}

/*
** Converte il contenuto di questa stringa in
** maiuscolo
*/
void lstring_toupper( lstring* str )
{
    int i;

    if( str->buf!=NULL ) {
    for ( i=0; i<str->len; i++ ) {
        str->buf[i] = toupper( str->buf[i] );
    }
    }    
}

/*
** Converte il contenuto di questa stringa in
** minuscolo
*/
void lstring_tolower( lstring* str )
{
    int i;

    if( str->buf!=NULL ) {
    for ( i=0; i<str->len; i++ ) {
        str->buf[i]=tolower(str->buf[i]);
    }
    }    
}    

/*
** In una certa stringa rimpiazza tutte le occorrenze di un 
** carattere con un'altro
*/
void lstring_replacechar( lstring* str, char orig, char dest )
{
    char *buf;

    buf = str->buf;
    if( buf!=NULL )
    {
        while( (*buf)!=0 )
        {
            if( (*buf)==orig )
            {
                (*buf)=dest;
            }
            buf = buf+1;
        }
    }
}

/*
** Controlla se una certa stringa ha un'altra stringa per suffisso
*/
int lstring_ends_with_cstr( lstring* str, const char *suffix )
{
    char *temp;
    int tempLen;
    
    if( str->buf==NULL )
    {
        return 0;
    } else
    {
        temp = strstr( str->buf, suffix );
        tempLen = strlen( suffix );
        if( temp==(str->buf + str->len - tempLen ) )
        {
            return 1;
        } else
        {
            return 0;
        }
    }
}

/*
** Prende l'ultimo indice di un certo carattere in una stringa. Se il
** carattere non appare restituisce -1
*/
int lstring_last_index_of( lstring* str, char c )
{
    int i;

    for( i=str->len-1; i>=0; i-- )
    {
        if( str->buf[i]==c )
        {
            return i;
        }    
    }

    return -1;
}

/*
** Tronca una stringa ad una certa lunghezza
*/
void lstring_truncate( lstring* str, int l )
{
    l_assert( str!=NULL );
    l_assert( l>=0 );

    if ( str->buf==NULL ) {
        return;
    }

    if( l<=str->len )
    {
        str->buf[l] = '\x0';
        str->len = l;
    }    
}

/*
** Azzera il contenuto di una stringa
*/
void lstring_reset( lstring* str )
{
    lstring_finalize( str );
}

/*
** Lunghezza di una stringa
*/
int lstring_len( lstring* str ) {
    return str->len;
}

/*
** Aggiunge alla stringa un preformattato sprintf
*/
void lstring_append_sprintf( lstring *str, const char *format, ... ) {
    char buffer[512];
    va_list args;

    va_start( args, format );
    vsnprintf( buffer, 511, format, args );
    va_end( args );

    buffer[511] = '0';
    lstring_append_cstr( str, buffer );
}
