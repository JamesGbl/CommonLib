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
#include "db_prepwrapper.h"
#include "lstring.h"
#include "lvector.h"
#include "lmemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct PrepWrapper {
    DbPrepared parent;
    DbConnection *origDc;
    lstring *sql;
    lstring *sqlDaEseguire;
    lvector *parametriCorrenti;
};
typedef struct PrepWrapper PrepWrapper;

static void PrepWrapper_destroy( DbPrepared *parent ) {
    int i;

    PrepWrapper *self = (PrepWrapper *)parent;
    lstring_delete( self->sql );
    lstring_delete( self->sqlDaEseguire );

    for ( i=0; i<lvector_len(self->parametriCorrenti); i++ ) {
        lstring_delete( (lstring*) lvector_at(self->parametriCorrenti, i) );
    }
    lvector_delete( self->parametriCorrenti );
}

static int PrepWrapper_dammi_numero_parametri( DbPrepared *parent ) {
    PrepWrapper *self = (PrepWrapper *)parent;
    return lvector_len( self->parametriCorrenti );
}

static void PrepWrapper_metti_parametro_intero( DbPrepared *parent, int n, int valore ) {
    PrepWrapper *self = (PrepWrapper *)parent;
    lstring *parametro = (lstring*)lvector_at( self->parametriCorrenti, valore );
    char buffer[64];

    if ( parametro==NULL ) return;
    sprintf( buffer, "%i", n );
    lstring_from_cstr( parametro, buffer );
}

static void PrepWrapper_metti_parametro_nullo( DbPrepared *parent, int n ) {
    PrepWrapper *self = (PrepWrapper *)parent;
    lstring *valore = (lstring*)lvector_at( self->parametriCorrenti, n );

    if ( valore==NULL ) return;
    lstring_from_cstr( valore, "NULL" );
}

static void PrepWrapper_metti_parametro_stringa( DbPrepared *parent, int n, const char *valore ) {
    PrepWrapper *self; 
    lstring *parametro;
    int i, len;

    if ( valore==NULL ) {
        PrepWrapper_metti_parametro_nullo( parent, n );
        return;
    }

    self = (PrepWrapper *)parent; 
    parametro = (lstring *)lvector_at( self->parametriCorrenti, n );
    if ( parametro==NULL ) return;
    len = strlen( valore );

    lstring_truncate( parametro, 0 );
    lstring_append_char( parametro, '\'' );
    for (i=0; i<len; i++) {
        if ( valore[i]=='\'' ) {
            lstring_append_cstr( parametro, "\'\'" );
        } else {
            lstring_append_char( parametro, valore[i] );
        }
    }
    lstring_append_char( parametro, '\'' );
}

static void PrepWrapper_ricalcola_sql( PrepWrapper *self ) {
    int len, nParametro, i;
    const char *sql;

    if ( self==NULL ) return;
    lstring_reset( self->sqlDaEseguire );

    len = lstring_len( self->sql );
    sql = lstring_to_cstr( self->sql );
    nParametro = 0;
    for ( i=0; i<len; i++ ) {
        if ( sql[i]=='?' ) {
            lstring_append_lstring( self->sqlDaEseguire, (lstring*)lvector_at( self->parametriCorrenti, nParametro ) );
            nParametro++;
        } else {
            lstring_append_char( self->sqlDaEseguire, sql[i] );
        }
    }
}

static int PrepWrapper_sql_exec( DbPrepared* parent, lerror **error ) {
    PrepWrapper *self;

    self = (PrepWrapper *)parent; 
    PrepWrapper_ricalcola_sql( self );

    return DbConnection_sql_exec( self->origDc, lstring_to_cstr( self->sqlDaEseguire ), error );
}

static DbIterator * PrepWrapper_sql_retrieve( DbPrepared* parent, lerror **error ) {
    PrepWrapper *self;

    self = (PrepWrapper *)parent; 
    PrepWrapper_ricalcola_sql( self );

    return DbConnection_sql_retrieve( self->origDc, lstring_to_cstr( self->sqlDaEseguire ), error );
}

DbPrepared *PrepWrapper_For( DbConnection* dc, const char *sql ) {
    static DbPrepared_class oClass;
    PrepWrapper *self;
    int quantiParametri;
    int len, i;

    oClass.destroy = PrepWrapper_destroy;
    oClass.dammi_numero_parametri = PrepWrapper_dammi_numero_parametri;
    oClass.metti_parametro_intero = PrepWrapper_metti_parametro_intero;
    oClass.metti_parametro_nullo = PrepWrapper_metti_parametro_nullo;
    oClass.metti_parametro_stringa = PrepWrapper_metti_parametro_stringa;
    oClass.sql_exec = PrepWrapper_sql_exec;
    oClass.sql_retrieve = PrepWrapper_sql_retrieve;

    self = lmalloc( sizeof(struct PrepWrapper) );
    DbPrepared_init( (DbPrepared*)self, &oClass );

    self->origDc = dc;
    self->sql = lstring_new_from_cstr( sql );
    self->sqlDaEseguire = lstring_new();

    len = lstring_len( self->sql );
    sql = lstring_to_cstr( self->sql );
    quantiParametri = 0;
    for ( i=0; i<len; i++ ) {
        if ( sql[i]=='?' ) quantiParametri++;
    }

    self->parametriCorrenti = lvector_new( quantiParametri );
    for (i=0; i<quantiParametri; i++ ) {
        lvector_set( self->parametriCorrenti, i, lstring_new() );
    }

    return (DbPrepared*)self;
}

