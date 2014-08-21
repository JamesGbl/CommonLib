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
#include "separatore_query.h"
#include <locale.h>
#include "lcross.h"
#include <ctype.h>

/**
 * Salta gli spazi in una query
 */
enum StatoAnalizzatoreCommenti {
    SAC_NORMALE,
    SAC_DENTRO_COMMENTO_RIGA,
    SAC_DENTRO_COMMENTO_TIPO_C,
    SAC_DENTRO_COMMENTO_TIPO_C_FINISCE
};

static const char *saltaSpazi( const char *c ) {
    enum StatoAnalizzatoreCommenti stato;
    
    stato = SAC_NORMALE;

    for( stato=SAC_NORMALE; *c; c++ ) {
	
	if ( stato==SAC_DENTRO_COMMENTO_RIGA ) {
	    if ( *c=='\n' ) {
		stato = SAC_NORMALE;
	    }

        } else if ( stato==SAC_DENTRO_COMMENTO_TIPO_C ) {
            if ( *c=='*' && *(c+1)=='/') {
                stato = SAC_DENTRO_COMMENTO_TIPO_C_FINISCE;
            }

        } else if ( stato==SAC_DENTRO_COMMENTO_TIPO_C_FINISCE ) {
            stato = SAC_NORMALE;

        } else {
            if ( *c=='-' && *(c+1)=='-' ) {
                stato = SAC_DENTRO_COMMENTO_RIGA;

            } else if ( *c=='/' && *(c+1)=='*') {

                stato = SAC_DENTRO_COMMENTO_TIPO_C;

            } else if ( isspace( *c ) ) {
		/* nop() */
            } else {
		break;
            }
        }
    }

    return c;
}

/**
 * Salta alla prossima istruzione in una query
 */
enum StatoAnalizzatoreSql {
    SAS_STATO_NORMALE,
    SAS_STATO_STRINGA,
    SAS_STATO_IDENTIFICATORE
};

static const char *saltaProssimaQuery( const char *sql, char terminatore ) {
    enum StatoAnalizzatoreSql stato;
    const char *c;

    for( stato=SAS_STATO_NORMALE, c=sql; *c; c++ ) {

        if ( stato==SAS_STATO_NORMALE ) {
            if ( *c==terminatore ) {
		break;
            } else if ( *c=='\"' ) {
                stato = SAS_STATO_IDENTIFICATORE;
            } else if ( *c=='\'') {
                stato = SAS_STATO_STRINGA;
            } else {
		/* nop() */
            }
        } else if ( stato==SAS_STATO_STRINGA ) {
            if ( *c=='\'' ) {
                stato = SAS_STATO_NORMALE;
            } else {
		/* nop() */
            }
        } else if ( stato==SAS_STATO_IDENTIFICATORE ) {
            if ( *c=='\"' ) {
                stato = SAS_STATO_NORMALE;
            } else {
		/* nop() */
            }
        } else {
            /* mai qua! */
	    l_assert( 0 );
	    /* nop(); */
        }
    }

    return c;
}

const char *DB_prossima_query( const char *sql ) {
    const char *fineQuery;
    const char *inizio;

    if ( sql==NULL ) return NULL;

    inizio = saltaSpazi( sql );
    fineQuery = saltaProssimaQuery( inizio, ';' );
    return fineQuery;
}

