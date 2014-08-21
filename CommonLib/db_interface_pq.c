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

#include "db_interface_pq.h"
#include "slist.h"
#include <string.h>
#include <stdlib.h>
#include "lcross.h"
#include <libpq-fe.h>

/**
 * Iteratore
 */

typedef struct DbIterator_Pq DbIterator_Pq;
struct DbIterator_Pq {
    struct DbIterator parent;
    int shared;
    int recNo;
    PGresult *res;
};

static void DbIteratorPq_destroy( DbIterator *parent ) {
    DbIterator_Pq *self = (DbIterator_Pq *)parent;

    if ( !self->shared ) {
	PQclear( self->res );
    }
}

static int DbIteratorPq_dammi_numero_campi( DbIterator *parent ) {
    DbIterator_Pq *self = (DbIterator_Pq *)parent;
    return PQnfields( self->res );
}

static const char *DbIteratorPq_dammi_nome_campo( DbIterator *parent, int i ) {
    DbIterator_Pq *self = (DbIterator_Pq *)parent;
    return PQfname( self->res, i );
}

static int DbIteratorPq_prossima_riga( DbIterator *parent ) {
    DbIterator_Pq *self = (DbIterator_Pq *)parent;

    self->recNo++;
    if ( self->recNo < PQntuples( self->res ) ) {
	return 1;
    } else {
	return 0;
    }
}

static const char * DbIteratorPq_dammi_valore( DbIterator *parent, int i ) {
    DbIterator_Pq *self = (DbIterator_Pq *)parent;

    l_assert( 0<=i );
    l_assert( i<PQnfields( self->res ) );

    if ( PQgetisnull( self->res, self->recNo, i ) ) {
	return "";
    } else {
	return PQgetvalue( self->res, self->recNo, i );
    }
}

static lbool DbIteratorPq_controlla_valore_nullo( DbIterator *parent, int i ) {
    DbIterator_Pq *self = (DbIterator_Pq *)parent;
    l_assert( 0<=i );
    l_assert( i<PQnfields( self->res ) );
    return PQgetisnull( self->res, self->recNo, i );
}

static DbIterator *crea_iteratore_pq_per( PGresult *res, int shared ) {
    static DbIterator_class oClass;
    DbIterator_Pq *self = NULL;

    self = malloc( sizeof( DbIterator_Pq ) );
    oClass.destroy = DbIteratorPq_destroy;
    oClass.dammi_numero_campi = DbIteratorPq_dammi_numero_campi;
    oClass.dammi_nome_campo = DbIteratorPq_dammi_nome_campo;
    oClass.prossima_riga = DbIteratorPq_prossima_riga;
    oClass.dammi_valore = DbIteratorPq_dammi_valore;
    oClass.controlla_valore_nullo = DbIteratorPq_controlla_valore_nullo;

    self->res = res;
    self->shared = shared;
    self->recNo = -1;

    DbIterator_init( (DbIterator *)self, &oClass );
    return (DbIterator *)self;
}

/**
 * Query preparata
 */
typedef struct DbPrepared_Pq DbPrepared_Pq;
struct DbPrepared_Pq {
    struct DbPrepared parent;
    PGconn *conn;
    lstring *prepName;
    slist *parametri;
    int *flagNulli;
    int quantiParametri;
};

void normalizzaParametriPg( lstring *result, const char *sql, int *quantiParametri ) {
    /* questa e' una normalizzazione un po' cosi' cosi' e suppone che non ci siano stringhe
     * all'interno della query da trattare
     */
    int i, len, lastParam;

    len = strlen( sql );

    for ( i=0, lastParam=0; i<len; i++ ) {
	if ( sql[i]=='?' ) {
	    /* occhio! i parametri, in PQ, partano da 1 */
	    lstring_append_sprintf( result, "$%d", lastParam+1 );
	    lastParam++;
	} else {
	    lstring_append_char( result, sql[i] );
	}
    }

    if ( quantiParametri!=NULL ) {
	*quantiParametri = lastParam;
    }
}

static void DbPreparedPq_destroy( DbPrepared *parent ) {
    DbPrepared_Pq *self = (DbPrepared_Pq *)parent;
    PGresult *res = NULL;
    lstring *queryDealloc;
    
    queryDealloc = lstring_new();
    lstring_append_sprintf( queryDealloc, "DEALLOCATE %s", lstring_to_cstr( self->prepName ) );
    res = PQexec( self->conn, lstring_to_cstr( queryDealloc ) );
    if ( res!=NULL ) {
	PQclear( res );
    }

    lstring_delete( self->prepName );
    lstring_delete( queryDealloc );
    slist_destroy( self->parametri );
    free( self->flagNulli );
}

static int DbPreparedPq_dammi_numero_parametri( DbPrepared *parent ) {
    DbPrepared_Pq *self = (DbPrepared_Pq *)parent;
    return self->quantiParametri;
}

static void DbPreparedPq_metti_parametro_nullo( DbPrepared* parent, int n ) {
    DbPrepared_Pq *self = (DbPrepared_Pq *)parent;
    if ( n<0 || n>=self->quantiParametri ) return;
    self->flagNulli[n] = 1;
}

static void DbPreparedPq_metti_parametro_stringa( DbPrepared *parent, int n, const char *valore ) {
    DbPrepared_Pq *self = (DbPrepared_Pq *)parent;

    if ( valore==NULL ) {
	DbPreparedPq_metti_parametro_nullo( parent, n );
	return;
    }

    if ( n<0 || n>=self->quantiParametri ) return;

    slist_set( self->parametri, n, valore );
    self->flagNulli[n] = 0;
}

static void DbPreparedPq_metti_parametro_intero( DbPrepared* parent, int n, int valore ) {
    char buffer[60];
    DbPrepared_Pq *self = (DbPrepared_Pq *)parent;
    if ( n<0 || n>=self->quantiParametri ) return;
    sprintf( buffer, "%d", valore );
    slist_set( self->parametri, n, buffer );
    self->flagNulli[n] = 0;
}

static lbool DbPreparedPq_sql_exec( DbPrepared *parent, lerror **error ) {
    DbPrepared_Pq *self = (DbPrepared_Pq *)parent;
    PGresult *res;
    const char **valori;
    int i;
    lbool result = LFALSE;

    l_assert( error==NULL || (*error)==NULL );

    valori = calloc(sizeof(char *), self->quantiParametri );

    for ( i=0; i<self->quantiParametri; i++ ) {
	if ( self->flagNulli[i] ) {
	    valori[i] = NULL;
	} else {
	    valori[i] = slist_at( self->parametri, i );
	}
    }

    res = PQexecPrepared( self->conn, lstring_to_cstr( self->prepName ),
			  self->quantiParametri, valori, NULL, NULL, 0 );

    if ( PQresultStatus(res)!=PGRES_COMMAND_OK && PQresultStatus(res)!=PGRES_TUPLES_OK ) {
	lerror_set( error, PQresultErrorMessage(res) );
	result = LFALSE;
    } else {
        result = LTRUE;
    }

    PQclear( res );
    free( valori );

    return result;
}

DbIterator* DbPreparedPq_sql_retrieve( DbPrepared *parent, lerror **error ) {
    DbPrepared_Pq *self = (DbPrepared_Pq *)parent;
    DbIterator *result = NULL;
    PGresult *res;
    const char **valori;
    int i;

    valori = calloc(sizeof(char *), self->quantiParametri );

    for ( i=0; i<self->quantiParametri; i++ ) {
	if ( self->flagNulli[i] ) {
	    valori[i] = NULL;
	} else {
	    valori[i] = slist_at( self->parametri, i );
	}
    }

    res = PQexecPrepared( self->conn, lstring_to_cstr( self->prepName ),
			  self->quantiParametri, valori, NULL, NULL, 0 );

    if ( PQresultStatus(res)!=PGRES_COMMAND_OK && PQresultStatus(res)!=PGRES_TUPLES_OK ) {
	lerror_set( error, PQresultErrorMessage(res) );
	result = NULL;
	PQclear( res );
    } else {
	result = crea_iteratore_pq_per( res, 0 );
    }

    free( valori );

    return result;
}

static DbPrepared_Pq *crea_prepared_per( PGconn *conn, int prepId, const char *sql, lerror **error ) {
    PGresult *res = NULL;
    lstring *sqlPq;
    DbPrepared_Pq *self;
    static DbPrepared_class oClass;

    if ( conn==NULL || sql==NULL) return NULL;

    oClass.destroy = DbPreparedPq_destroy;
    oClass.dammi_numero_parametri = DbPreparedPq_dammi_numero_parametri;
    oClass.metti_parametro_stringa = DbPreparedPq_metti_parametro_stringa;
    oClass.metti_parametro_intero = DbPreparedPq_metti_parametro_intero;
    oClass.metti_parametro_nullo = DbPreparedPq_metti_parametro_nullo;
    oClass.sql_exec = DbPreparedPq_sql_exec;
    oClass.sql_retrieve = DbPreparedPq_sql_retrieve;

    self = malloc( sizeof(DbPrepared_Pq) );
    self->conn = conn;
    self->prepName = lstring_new();
    lstring_append_sprintf( self->prepName, "mprep_%d", prepId );

    sqlPq = lstring_new();
    normalizzaParametriPg( sqlPq, sql, &self->quantiParametri );
    self->parametri = slist_new( self->quantiParametri );
    self->flagNulli = malloc( sizeof(int) * self->quantiParametri );

    res = PQprepare( self->conn, 
		     lstring_to_cstr( self->prepName ), 
		     lstring_to_cstr( sqlPq ), 
		     self->quantiParametri,
		     NULL );

    if ( res==NULL || PQresultStatus( res )!=PGRES_COMMAND_OK ) {
	lerror_set( error, PQresultErrorMessage(res) );
	lstring_delete( self->prepName );
	slist_destroy( self->parametri );
	free( self->flagNulli );
	free( self );
	self = NULL;
    } else {
	DbPrepared_init( (DbPrepared *)self, &oClass );
    }

    PQclear( res );
    lstring_delete( sqlPq );

    return self;
}

/**
 * Connessione PQ 
 */

typedef struct DbConnection_Pq DbConnection_Pq;
struct DbConnection_Pq {
    struct DbConnection parent;
    PGconn *conn;
    int lastPrep;
};

lbool DbConnectionPq_sql_exec( DbConnection *parent, const char *sql, lerror **error ) {
    PGresult *res = NULL;
    DbConnection_Pq *self = (DbConnection_Pq *)parent;
    lbool result = LFALSE;

    l_assert( sql!=NULL );

    res = PQexec( self->conn, sql );
    if ( res==NULL ) {
	lerror_set( error, "Libpq memory allocation error (libpq)" );
	return LFALSE;
    }

    if ( PQresultStatus(res)!=PGRES_TUPLES_OK && PQresultStatus(res)!=PGRES_COMMAND_OK ) {
	lstring_from_cstr( parent->lastError, PQresultErrorMessage( res ) );
	lerror_set( error, PQresultErrorMessage( res ) );
	result = LFALSE;
    } else {
	lstring_truncate( parent->lastError, 0 );
	result = LTRUE;
    }

    PQclear( res );

    return result;
}

void DbConnectionPq_destroy( DbConnection *parent ) {
    DbConnection_Pq *self = (DbConnection_Pq *)parent;
    PQfinish( self->conn );
}

DbIterator * DbConnectionPq_sql_retrieve( DbConnection *parent, const char * sql, lerror **error ) {
    PGresult *res = NULL;
    DbConnection_Pq *self = (DbConnection_Pq *)parent;
    DbIterator *result = NULL;

    if ( sql==NULL ) return NULL;
    res = PQexec( self->conn, sql );
    if ( res==NULL ) return NULL;

    if ( PQresultStatus(res)!=PGRES_TUPLES_OK && PQresultStatus(res)!=PGRES_COMMAND_OK ) {
	lstring_from_cstr( parent->lastError, PQresultErrorMessage( res ) );
	lerror_set( error, PQresultErrorMessage(res) );
	PQclear( res );
	result = NULL;
    } else {
	lstring_truncate( parent->lastError, 0 );
	result = crea_iteratore_pq_per( res, 0 );
    }
    
    return result;
}

DbPrepared * DbConnectionPq_sql_prepare( DbConnection *parent, const char *sql, lerror **error ) {
    DbConnection_Pq *self = (DbConnection_Pq *)parent;

    self->lastPrep++;
    return (DbPrepared *) crea_prepared_per( self->conn, self->lastPrep, sql, error );
}

DbConnection *DbConnection_Pq_new( const char *connString, lerror **error ) {
    DbConnection_Pq *self;
    static DbConnection_class oClass;
    PGconn *conn;

    if ( connString==NULL ) {
	lerror_set( error, "Connessione a PostgreSQL: stringa di connessione nulla?" );
	return NULL;
    }

    conn = PQconnectdb( connString );
    if ( conn==NULL ) {
	lerror_set( error, "Connessione a PostgreSQL: errore nell'allocazione della memoria" );
	return NULL;
    }

    if ( PQstatus(conn) != CONNECTION_OK ) {
	lerror_set( error, PQerrorMessage(conn) );
	PQfinish( conn );
	return NULL;
    }

    /* inizializzazione dei metodi di classe */
    oClass.destroy = DbConnectionPq_destroy;
    oClass.sql_exec = DbConnectionPq_sql_exec;
    oClass.sql_retrieve = DbConnectionPq_sql_retrieve;
    oClass.sql_prepare = DbConnectionPq_sql_prepare;

    self = (DbConnection_Pq *)malloc( sizeof(struct DbConnection_Pq) );
    self->conn = conn;
    self->lastPrep = 0;

    DbConnection_init( (DbConnection *)self, &oClass );
    return (DbConnection *)self;
}
