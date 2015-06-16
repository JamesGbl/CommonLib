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
#include <stdlib.h>
#include "lcross.h"
#include "third-party/sqlite3.h"
#include "lmemory.h"
#include "db_interface_sqlite.h"

DbIterator_class* DbIterator_Sqlite_class();
DbPrepared_class* DbPrepared_Sqlite_class();
DbConnection_class* DbConnection_Sqlite_class();

/* DbIterator_Sqlite {{{ */

struct DbIterator_Sqlite {
	DbIterator parent;
	sqlite3 *db;
	sqlite3_stmt *statement;
	int shared;
};
typedef struct DbIterator_Sqlite DbIterator_Sqlite;

DbIterator_Sqlite *DbIterator_alloc_sqlite( DbConnection *connection, sqlite3 *pDb, sqlite3_stmt* pStatement, int pShared, lerror **error ) {
	DbIterator_Sqlite *self = (DbIterator_Sqlite *)lmalloc( sizeof(DbIterator_Sqlite) );

	DbIterator_init( connection, (DbIterator *)self, DbIterator_Sqlite_class() );
	self->db = pDb;
	self->statement = pStatement;
	self->shared = pShared;

	return self;
}

void DbIterator_Sqlite_destroy( DbIterator *parent ) {

	DbIterator_Sqlite *self = (DbIterator_Sqlite*)parent;
	if ( !self ) return;

	if ( !self->shared ) {
		sqlite3_finalize( self->statement );
	}
}

int DbIterator_Sqlite_dammi_numero_campi( DbIterator *iter ) {
	DbIterator_Sqlite *self = (DbIterator_Sqlite *)iter;
	return sqlite3_column_count( self->statement );
}

const char * DbIterator_Sqlite_dammi_nome_campo( DbIterator *iter, int i ) {
	DbIterator_Sqlite *self = (DbIterator_Sqlite *)iter;
	return sqlite3_column_name( self->statement, i );
}

int DbIterator_Sqlite_prossima_riga( DbIterator *iter ) {
	DbIterator_Sqlite *self = (DbIterator_Sqlite *)iter;
	int status = sqlite3_step( self->statement );
	switch( status ) {
	case SQLITE_DONE:
		return 0;
	case SQLITE_ROW:
		return 1;
	default:
		/* Condizione di errore */
		return 0;
	}
}

static const char * DbIterator_Sqlite_dammi_valore( DbIterator *iter, int i ) {
	DbIterator_Sqlite *self = (DbIterator_Sqlite *)iter;
	const char *valore = NULL;

	l_assert( 0<=i );
	l_assert( i<sqlite3_column_count(self->statement) );

	valore = (const char *)sqlite3_column_text( self->statement, i );
	if ( valore==NULL ) {
		return "";
	} else {
		return valore;
	}
}

static lbool DbIterator_Sqlite_controlla_valore_nullo( DbIterator *iter, int i ) {
	DbIterator_Sqlite *self = (DbIterator_Sqlite *)iter;
	const char *valore = NULL;

	l_assert( 0<=i );
	l_assert( i<sqlite3_column_count(self->statement) );

	valore = (const char *)sqlite3_column_text( self->statement, i );
	return ( valore==NULL );
}

DbIterator_class * DbIterator_Sqlite_class() {
	static DbIterator_class oClass;

	oClass.destroy = DbIterator_Sqlite_destroy;
	oClass.dammi_numero_campi = DbIterator_Sqlite_dammi_numero_campi;
	oClass.dammi_nome_campo = DbIterator_Sqlite_dammi_nome_campo;
	oClass.prossima_riga = DbIterator_Sqlite_prossima_riga;
	oClass.dammi_valore = DbIterator_Sqlite_dammi_valore;
	oClass.controlla_valore_nullo = DbIterator_Sqlite_controlla_valore_nullo;

	return &oClass;
}
/* }}} */

/* DbPrepared_Sqlite {{{ */

struct DbPrepared_Sqlite {
	DbPrepared parent;
	sqlite3 *db;
	sqlite3_stmt *statement;
	lstring *sql;
};
typedef struct DbPrepared_Sqlite DbPrepared_Sqlite;

void DbPrepared_Sqlite_destroy( DbPrepared* parent ) {    
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)parent;
	lstring_delete( self->sql );
	sqlite3_finalize( self->statement );
}

int DbPrepared_Sqlite_dammi_numero_parametri( DbPrepared *parent ) {
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)parent;
	return sqlite3_bind_parameter_count( self->statement );
}

void DbPrepared_Sqlite_metti_parametro_intero( DbPrepared *parent, int n, int valore ) {
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)parent;
	l_assert( 0<=n && n< sqlite3_bind_parameter_count( self->statement ) );
	sqlite3_bind_int( self->statement, n+1, valore );
}

void DbPrepared_Sqlite_metti_parametro_nullo( DbPrepared *parent, int n ) {
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)parent;
	l_assert( 0<=n && n< sqlite3_bind_parameter_count( self->statement ) );
	sqlite3_bind_null( self->statement, n+1 );
}

void DbPrepared_Sqlite_metti_parametro_stringa( DbPrepared *parent, int n, const char *valore ) {
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)parent;
	l_assert( 0<=n && n< sqlite3_bind_parameter_count( self->statement ) );
	sqlite3_bind_text( self->statement, n+1, valore, -1, SQLITE_TRANSIENT );
}

int DbPrepared_Sqlite_sql_exec( DbPrepared *parent, lerror **error ) {
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)parent;
	int retval = 1;

	int status = sqlite3_step( self->statement );
	if ( status==SQLITE_ROW ) {
		lerror_set( error, "sql_exec chiamato con una SELECT" );
		self->parent.lastError = lstring_from_cstr_f( self->parent.lastError, "sql_exec chiamato con una SELECT" );
		retval = 0;
	} else if ( status!=SQLITE_DONE ) {
		const char *errMsg = sqlite3_errmsg( self->db );
		if ( errMsg!=NULL ) {
			lerror_set_sprintf( error, "SQLite error: %s\nQuery was: %s", errMsg, self->sql);
			self->parent.lastError = lstring_from_cstr_f( self->parent.lastError, errMsg );
		} else {
			lerror_set( error, "Tipo di errore SQLite sconosciuto" );
			self->parent.lastError = lstring_from_cstr_f( self->parent.lastError, "Tipo di errore sconosciuto" );
		}
		retval = 0;
	}

	sqlite3_reset( self->statement );
	return retval;
}

DbIterator * DbPrepared_Sqlite_sql_retrieve( DbPrepared *parent, lerror **error ) {
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)parent;
	return (DbIterator *)DbIterator_alloc_sqlite( parent->originatingConnection, self->db, self->statement, 1, error );
}

DbPrepared_class * DbPrepared_Sqlite_class() {
	static DbPrepared_class oClass;

	oClass.destroy = DbPrepared_Sqlite_destroy;
	oClass.dammi_numero_parametri = DbPrepared_Sqlite_dammi_numero_parametri;
	oClass.metti_parametro_intero = DbPrepared_Sqlite_metti_parametro_intero;
	oClass.metti_parametro_nullo = DbPrepared_Sqlite_metti_parametro_nullo;
	oClass.metti_parametro_stringa = DbPrepared_Sqlite_metti_parametro_stringa;
	oClass.sql_exec = DbPrepared_Sqlite_sql_exec;
	oClass.sql_retrieve = DbPrepared_Sqlite_sql_retrieve;

	return &oClass;
}

DbPrepared_Sqlite * DbPrepared_Sqlite_alloc( DbConnection *connection, sqlite3 *pDb, const char *sql, sqlite3_stmt *pStatement ) {
	DbPrepared_Sqlite *self = (DbPrepared_Sqlite *)lmalloc( sizeof(DbPrepared_Sqlite) );
	DbPrepared_init( connection, (DbPrepared *)self, DbPrepared_Sqlite_class() );
	self->sql = lstring_new_from_cstr( sql );
	self->db = pDb;
	self->statement = pStatement;
	return self;
}

/* }}} */

/* DbConnection_Sqlite {{{ */
struct DbConnection_Sqlite {
	DbConnection parent;
	sqlite3 *db;
	lbool shared;
};
typedef struct DbConnection_Sqlite DbConnection_Sqlite;

DbConnection_Sqlite *DbConnection_Sqlite_new( const char *nomeFile, lerror **error ) {
	DbConnection_Sqlite *self = NULL;
	int rc;

	l_assert(error==NULL || *error==NULL);

	self = (DbConnection_Sqlite *)lmalloc( sizeof(DbConnection_Sqlite) );
	DbConnection_init( (DbConnection *)self, DbConnection_Sqlite_class() );

	self->shared = LFALSE;
	rc = sqlite3_open_v2( nomeFile, &self->db, SQLITE_OPEN_URI | SQLITE_OPEN_READWRITE, NULL );
	if (rc!=SQLITE_OK) {
		if ( error!=NULL) {
			lerror_set(error, sqlite3_errstr(rc));
		}
		lfree(self);
		self = NULL;
	}

	return self;
}

DbConnection_Sqlite *DbConnection_Sqlite_new_mem_shared(const char *dbname) {
	DbConnection_Sqlite *result = NULL;
	lstring *databaseUri = lstring_new();

	databaseUri = lstring_append_cstr_f(databaseUri, "file:");
	databaseUri = lstring_append_cstr_f(databaseUri, dbname);
	databaseUri = lstring_append_cstr_f(databaseUri, "?mode=memory&cache=shared");
	result = (DbConnection_Sqlite *)DbConnection_Sqlite_new(databaseUri, NULL);

	return result;
}

DbConnection_Sqlite *DbConnection_Sqlite_new_mem( void ) {
	return DbConnection_Sqlite_new( ":memory:", NULL );
}


DbConnection_Sqlite *DbConnection_Sqlite_new_embed(void *handle)
{
    DbConnection_Sqlite *self = lmalloc( sizeof(DbConnection_Sqlite) );
    DbConnection_init( (DbConnection *)self, DbConnection_Sqlite_class() );

    self->db = handle;
    self->shared = LTRUE;

    return self;
}

void DbConnection_Sqlite_destroy( DbConnection *parent ) {
	DbConnection_Sqlite *self = (DbConnection_Sqlite *)parent;
	if (!self->shared) {
		sqlite3_close( self->db );
	}
}

lbool DbConnection_Sqlite_sql_exec( DbConnection *parent, const char *sql, lerror **error ) {
	DbConnection_Sqlite *self = (DbConnection_Sqlite *)parent;
	char *errMsg;
	lbool retval = LTRUE;

	if ( SQLITE_OK != sqlite3_exec( self->db, sql, NULL, NULL, &errMsg) ) {
		retval = LFALSE;

		if ( errMsg!=NULL ) {
			parent->lastError = lstring_from_cstr_f( parent->lastError, errMsg );
			lerror_set( error, errMsg );
			sqlite3_free( errMsg );
		} else {
			lerror_set( error, "Sqlite unknown error" );
			parent->lastError = lstring_from_cstr_f( parent->lastError, "Tipo di errore sconosciuto" );
		}
	}

	return retval;
}

DbIterator* DbConnection_Sqlite_sql_retrieve( DbConnection *parent, const char *sql, lerror **error ) {
	DbConnection_Sqlite *self = (DbConnection_Sqlite *)parent;
	DbIterator *retval = NULL;
	sqlite3_stmt *statement;

	if ( SQLITE_OK != sqlite3_prepare_v2( self->db, sql, -1, &statement, NULL ) ) {
		const char *errMsg = sqlite3_errmsg( self->db );

		if ( errMsg!=NULL ) {
			lerror_set( error, errMsg );
			parent->lastError = lstring_from_cstr_f( parent->lastError, errMsg );
		} else {
			lerror_set( error, "Tipo di errore non conosciuto" );
			parent->lastError = lstring_from_cstr_f( parent->lastError, "Tipo di errore sconosciuto" );
		}
	} else {
		retval = (DbIterator *)DbIterator_alloc_sqlite( parent, self->db, statement, 0, error );
		if ( !retval ) {
			lstring_reset( parent->lastError );
		}
	}

	return retval;
}

DbPrepared *DbConnection_Sqlite_sql_prepare( DbConnection *parent, const char *sql, lerror **error ) {
	DbConnection_Sqlite *self = (DbConnection_Sqlite *)parent;
	sqlite3_stmt *statement;

	if ( SQLITE_OK != sqlite3_prepare_v2( self->db, sql, -1, &statement, NULL ) ) {
		const char *errMsg = sqlite3_errmsg( self->db );

		if ( errMsg!=NULL ) {
			lerror_set( error, errMsg );
			parent->lastError = lstring_from_cstr_f( parent->lastError, errMsg );
		} else {
			lerror_set( error, "Unknown SQLite error message" );
			parent->lastError = lstring_from_cstr_f( parent->lastError, "Tipo di errore sconosciuto" );
		}

		return NULL;
	} else {
		lstring_reset( parent->lastError );
	}

	return (DbPrepared *)DbPrepared_Sqlite_alloc( parent, self->db, sql, statement );
}

const char *DbConnection_Sqlite_get_type(DbConnection *parent) {
    l_assert(parent!=NULL);
	return SQLITE_CONNECTION_TYPE;
}

DbConnection_class *DbConnection_Sqlite_class() {
	static DbConnection_class oClass;

	oClass.destroy = DbConnection_Sqlite_destroy;
	oClass.sql_exec = DbConnection_Sqlite_sql_exec;
	oClass.sql_prepare = DbConnection_Sqlite_sql_prepare;
	oClass.sql_retrieve = DbConnection_Sqlite_sql_retrieve;
    oClass.get_type = DbConnection_Sqlite_get_type;

	return &oClass;
}

lbool DbConnection_export_to_sqlite( DbConnection_Sqlite *self, const char *fileName, lerror **error ) {
	DbConnection *parent = (DbConnection *)self;
	sqlite3 *otherDb;
	sqlite3_backup *backup;

	lstring_reset( parent->lastError );

	/* Questo serve per sbloccare il database
	*  in modo che si possa fare il backup.
	* Se questa query molla errore allora vuol dire che
	* non c'era alcuna transazione in corso e questo non e' un problema
	*/
	DbConnection_sql_exec( (DbConnection *)self, "commit", NULL );

	if ( SQLITE_OK!=sqlite3_open( fileName, &otherDb ) ) {
		const char *errMsg = sqlite3_errmsg( otherDb );
		if ( errMsg!=NULL ) {
			lerror_set( error, errMsg );
		} else {
			lerror_set( error, "Unknown SQLite error" );
		}

		sqlite3_close( otherDb );
		return 0;
	}

	backup = sqlite3_backup_init( otherDb, "main", self->db, "main" );
	if ( !backup ) {
		sqlite3_close( otherDb );
		lerror_set( error, "Can't lock the source database" );
		return 0;
	}

	sqlite3_backup_step( backup, -1 );
	sqlite3_backup_finish( backup );

	if ( SQLITE_OK != sqlite3_errcode( otherDb ) ) {
		const char *errMsg = sqlite3_errmsg( otherDb );
		if ( errMsg!=NULL ) {
			lerror_set( error, errMsg );
		} else {
			lerror_set( error, "Unknown error" );
		}

		sqlite3_close( otherDb );
		return 0;
	}

	sqlite3_close( otherDb );
	return 1;
}

void *DbConnection_Sqlite_get_handle(DbConnection_Sqlite *conn) {
	return conn->db;
}
