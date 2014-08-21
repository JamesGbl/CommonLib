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
#include "db_interface.h"
#include "lstring.h"
#include "lcross.h"
#include <stdlib.h>

/* DbIterator
 * ========== */

void DbIterator_init( DbIterator *self, DbIterator_class *oClass ) {
    l_assert( self!=NULL ); 
    self->oClass = oClass;
}

void DbIterator_destroy( DbIterator *self ) {
    if ( !self ) return; 
    self->oClass->destroy( self );
    free( self );
}

int DbIterator_dammi_numero_campi( DbIterator *self ) {
    l_assert( self!=NULL );
    return self->oClass->dammi_numero_campi( self );
}

const char * DbIterator_dammi_nome_campo( DbIterator *self, int i ) {
    l_assert( self!=NULL );
    return self->oClass->dammi_nome_campo( self, i );
}

int DbIterator_prossima_riga( DbIterator *self ) {
    l_assert( self!=NULL );
    return self->oClass->prossima_riga( self );
}

lbool DbIterator_controlla_valore_nullo( DbIterator *self, int i ) {
    l_assert( self!=NULL );
    return self->oClass->controlla_valore_nullo( self, i );
}

const char * DbIterator_dammi_valore( DbIterator *self, int i ) {
    if ( !self ) return NULL;
    return self->oClass->dammi_valore( self, i );
}


/* DbPrepared
 * ========== */

void DbPrepared_init( DbPrepared *self, DbPrepared_class *oClass ) {
    self->oClass = oClass;
    self->buffer = lstring_new();
    self->lastError = lstring_new();
}

void DbPrepared_destroy( DbPrepared *self ) {
    if ( !self ) return;
    self->oClass->destroy( self );

    lstring_delete( self->buffer );
    lstring_delete( self->lastError );

    free( self );
}

const char * DbPrepared_sql_into( DbPrepared *self, lerror **error ) {
    DbIterator *iter = DbPrepared_sql_retrieve( self, error );
    if ( iter==NULL ) {
        return NULL;	
    }

    if ( !DbIterator_prossima_riga( iter ) ) {
        lstring_from_cstr( self->buffer, "" );
    } else if ( DbIterator_dammi_numero_campi( iter )!=1 ) {
        lstring_from_cstr( self->buffer, "" );
    } else {
        lstring_from_cstr( self->buffer, DbIterator_dammi_valore( iter, 0 ) );
    }

    DbIterator_destroy( iter );

    return lstring_to_cstr( self->buffer );
}

const char * DbPrepared_last_error( DbPrepared *self ) {
    return lstring_to_cstr( self->lastError );
}

int DbPrepared_dammi_numero_parametri( DbPrepared* self ) {
    if ( !self ) return 0;
    return self->oClass->dammi_numero_parametri( self );
}

void DbPrepared_metti_parametro_intero( DbPrepared* self, int n, int valore ) {
    if ( !self) return;
    self->oClass->metti_parametro_intero( self, n, valore );
}

void DbPrepared_metti_parametro_nullo( DbPrepared* self, int n ) {
    if ( !self ) return;
    self->oClass->metti_parametro_nullo( self, n );
}

void DbPrepared_metti_parametro_stringa( DbPrepared* self, int n, const char *valore ) {
    if ( !self ) return;
    self->oClass->metti_parametro_stringa( self, n, valore );
}

int DbPrepared_sql_exec( DbPrepared *self, lerror **error ) {
    l_assert( self!=NULL );
    return self->oClass->sql_exec( self, error );
}

DbIterator *DbPrepared_sql_retrieve( DbPrepared *self, lerror **error ) {
    l_assert( self!=NULL );
    return self->oClass->sql_retrieve( self, error );
}

/* DbConnection
 * ============ */

void DbConnection_init( DbConnection *self, DbConnection_class *oClass ) {
    self->oClass = oClass;
    self->lastError = lstring_new();
    self->buffer = lstring_new();
}

void DbConnection_destroy( DbConnection *self ) {
    if ( !self ) return;

    self->oClass->destroy( self );
    lstring_delete( self->lastError );
    lstring_delete( self->buffer );
    free( self );
}

lbool DbConnection_sql_exec( DbConnection *self, const char *sql, lerror **error ) {
    l_assert( self!=NULL );
    return self->oClass->sql_exec( self, sql, error );
}

DbIterator* DbConnection_sql_retrieve( DbConnection *self, const char *sql, lerror **error ) {
    l_assert( self!=NULL );
    return self->oClass->sql_retrieve( self, sql, error );
}

DbPrepared *DbConnection_sql_prepare( DbConnection *self, const char *sql, lerror **error ) {
    l_assert( self!=NULL );
    l_assert( sql!=NULL );
    return self->oClass->sql_prepare( self, sql, error );
}

const char * DbConnection_sql_into( DbConnection *self, const char *sql, lerror **error ) {
    DbIterator *iter = DbConnection_sql_retrieve( self, sql, error );
    if ( !iter ) {
	return NULL;
    }

    if ( !DbIterator_prossima_riga( iter ) ) {
        lstring_from_cstr( self->buffer, "");
    } else if ( DbIterator_dammi_numero_campi( iter )!=1 ) {
        lstring_from_cstr( self->buffer, "");
    } else {
        lstring_from_cstr( self->buffer, DbIterator_dammi_valore( iter, 0 ) );
    }

    DbIterator_destroy( iter );
    return lstring_to_cstr( self->buffer );
}

const char * DbConnection_last_error_message( DbConnection *self ) {
    return lstring_to_cstr( self->lastError );
}
