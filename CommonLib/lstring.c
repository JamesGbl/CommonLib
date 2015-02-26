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
#include "lstring.h"
#include "lcross.h"
#include "lmemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/*
** Costruisce una nuova stringa vuota
*/
lstring* lstring_new( void )
{
	lstring_header* str_header = (lstring_header *)lmalloc(32);
	lstring *str = ((lstring *)str_header) + sizeof(lstring_header);
	str_header->bufLen = 32-sizeof(struct lstring_header);
	str_header->len = 0;
	str[0]='\x0';
	return str;
}

/*
** Costruisce una nuova stringa data la relativa
** stringa C
*/
lstring* lstring_new_from_cstr( const char *cstr )
{
	int len = strlen(cstr);	
	int bufLen = 2<<l_log2(len + 50 + sizeof(struct lstring_header));
	lstring_header* str = (lstring_header *)lmalloc(bufLen);
	lstring* s = ((char *)str)+sizeof(lstring_header);
	str->len = len;
	str->bufLen = bufLen - sizeof(struct lstring_header);
	l_strcpy(s, cstr);
    return s;
}    

/*
** Costruisce una nuova stringa prendendone
** un'altra nella stessa struttura
*/
lstring* lstring_new_from_lstr( lstring* self )
{
	lstring_header *self_header = (lstring_header*)(self-sizeof(struct lstring_header));
	lstring_header *str = (lstring_header *)lmalloc(sizeof(struct lstring_header) + self_header->len);
	lstring* s = ((char *)str)+sizeof(lstring_header);
	memcpy(str, self_header, self_header->bufLen + sizeof(struct lstring_header));
	return s;
}

/*
** Reinizializza una lstring a partire da una stringa
** C
*/
lstring *lstring_from_cstr_f( lstring* str, const char *other )
{
    lstring_truncate( str, 0 );
    if ( other!=NULL ) {
        return lstring_append_generic_f( str, other, strlen(other) );
    } else {
		return str;
	}
}

/*
** Reinizializza una lstring a partire da una lstring
*/
lstring* lstring_from_lstr_f( lstring* str, lstring *other )
{
	lstring_header *other_header = (lstring_header*)(other-sizeof(struct lstring_header));

    l_assert( str!=NULL );
    l_assert( other!=NULL );

    lstring_truncate( str, 0 );
	return lstring_append_generic_f( str, other, other_header->len );
}

/*
** Dealloca una stringa. Dopo questa chiamata la stringa
** non e' piu' valida
*/
void lstring_delete( lstring* str )
{
	if (str==NULL) return;
	lfree(str-sizeof(struct lstring_header));
}    

lstring *lstring_append_generic_f( lstring* str, const char *other, int otherLen)
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
    int newLen = otherLen + str_header->len;
    int newBufLen = 2<<l_log2(newLen + 50);

    if( str_header->bufLen > newBufLen )
    {
        newBufLen = str_header->bufLen;
    }    

    if( str_header->bufLen<newBufLen )
    {
		str_header = (lstring_header *)lrealloc(str_header, newBufLen+sizeof(lstring_header));
		str = ((lstring*)str_header)+sizeof(lstring_header);
    }    

    memcpy( str+str_header->len, other, otherLen );
    str[str_header->len+otherLen]=0;
    str_header->len = newLen;
    str_header->bufLen = newBufLen;

	return str;
}

/*
** Aggiunge, a una stringa, una stringa C.
** \param str Stringa alla quale aggiungere
** \param other Stringa da aggiungere
*/
lstring *lstring_append_cstr_f( lstring* str, const char *other )
{
    int otherLen;

    l_assert( str!=NULL );
    l_assert( other!=NULL );

    otherLen = strlen(other);
    return lstring_append_generic_f( str, other, otherLen );
}

/*
** Aggiunge un carattere
*/
lstring* lstring_append_char_f( lstring* str, char c ) {
    l_assert( str!=NULL );

    return lstring_append_generic_f( str, &c, 1 );
}

/*
** Aggiunge, a una stringa, una stringa.
** \param str Stringa alla quale aggiungere
** \param other Stringa da aggiungere
*/
lstring* lstring_append_lstring_f( lstring* str, lstring* other )
{
	lstring_header *other_header = (lstring_header *)(other - sizeof(lstring_header));
    l_assert( str!=NULL );
    l_assert( other!=NULL );
    return lstring_append_generic_f( str, other, other_header->len );
}    

/*
** Prepara il buffer di una stringa a disporre
** di almeno un certo numero di caratteri
** \param str La stringa da preparare
** \param len Il numero di caratteri
*/
lstring* lstring_reserve_f( lstring* str, int len )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
    l_assert( str!=NULL );
    if( str_header->bufLen<len )
    {
		str_header = (lstring_header *)lrealloc(str_header, len+sizeof(lstring_header));
		str = ((lstring*)str_header)+sizeof(lstring_header);
    }

	return str;
}    

/*
** Pulisce tutti gli spazi all'inizio di una stringa
** \param str Stringa da pulire
*/
void lstring_ltrim( lstring* str )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
    int i;

    l_assert( str!=NULL );

    if( str_header->len==0 ) return;

    i = 0;
    while( str[i] != 0 && isspace(str[i]) )
    {
        i++;
    }

    if( i>0 )
    {    
        str_header->len = str_header->len - i;
        memmove( str, str+i, str_header->len );
        str[str_header->len] = 0;
    }
}

/*
** Pulisce tutti gli spazi alla fine di una stringa
** \param str La stringa da pulire
*/
void lstring_rtrim( lstring* str )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));

    l_assert( str!=NULL );
    if( str_header->len==0 ) return;

    while( str_header->len > 0 && isspace(str[str_header->len-1]) )
    {
        str_header->len = str_header->len-1;
    }  

    str[str_header->len] = 0;
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

	if( strstr( str, other )!=NULL )
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

    if( 0==strcmp(str, cstr) )
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
	lstring_header *str1_header = (lstring_header *)(str_1 - sizeof(lstring_header));
	lstring_header *str2_header = (lstring_header *)(str_2 - sizeof(lstring_header));

    l_assert( str_1!=NULL );
    l_assert( str_2!=NULL );

    if( str_1==NULL )
    {
        return str_2==NULL;
    } else if( str_2==NULL )
    {
        return 0;
    } else if( str1_header->len != str2_header->len )
    {
        return 0;
    } else if( str_1==str_2 )
    {
        return 1;
    } else if( 0==memcmp(str_1, str_2, str1_header->len) )
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
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));

    if( str!=NULL ) {
		for ( i=0; i<str_header->len; i++ ) {
			str[i] = toupper( str[i] );
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
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));

    if( str!=NULL ) {
		for ( i=0; i<str_header->len; i++ ) {
			str[i]=tolower(str[i]);
		}
    }    
}    

/*
** In una certa stringa rimpiazza tutte le occorrenze di un 
** carattere con un'altro
*/
void lstring_replacechar( lstring* str, char orig, char dest )
{
    char *buf = str;

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
    
    if( str==NULL )
    {
        return 0;
    } else
    {
        temp = strstr( str, suffix );
        tempLen = strlen( suffix );
        if( temp==(str + lstring_len(str) - tempLen ) )
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

    for( i=lstring_len(str)-1; i>=0; i-- )
    {
        if( str[i]==c )
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
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
    l_assert( str!=NULL );
    l_assert( l>=0 );

	if( l<=str_header->len )
    {
        str[l] = '\x0';
        str_header->len = l;
    }    
}

/*
** Azzera il contenuto di una stringa
*/
void lstring_reset( lstring* str )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	str_header->len=0;
	str[0]='\x0';
}

/*
** Lunghezza di una stringa
*/
int lstring_len( lstring* str ) {
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
    return str_header->len;
}

/*
** Aggiunge alla stringa un preformattato sprintf
*/
lstring* lstring_append_sprintf_f( lstring *str, const char *format, ... ) {
    va_list args;
    char buffer[512];

    va_start( args, format );
    l_vsnprintf( buffer, 511, format, args );
    va_end( args );

    buffer[511] = '0';
    return lstring_append_cstr_f( str, buffer );
}

/*
** Check the empty string
*/
lbool lstring_empty(lstring *str) {
	return lstring_len(str) == 0;
}

/*
** Find a characted in a string
*/
int lstring_index_of_char(lstring *str, int startidx, char p) {
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	int i;

	l_assert(str!=NULL);
	l_assert(startidx>=0);

	if (startidx >= str_header->len) {
		return -1;
	}

	for (i=startidx; i<str_header->len; i++) {
		if (str[i]==p) {
			return i;
		}
	}

	return -1;
}

