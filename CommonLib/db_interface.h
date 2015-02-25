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

#ifndef __DB_INTERFACE_H
#define __DB_INTERFACE_H

#include "lcross.h"
#include "lstring.h"
#include "lerror.h"

/**
 * File: db_interface.h
 */

typedef struct DbIterator DbIterator;

/**
 * Class: DbConnection
 * 
 * This class represent an abstract database connection. You can
 * create you database connections using the appropriate
 * constructor functions.
 */
typedef struct DbConnection DbConnection;
typedef struct DbConnection_class DbConnection_class;
typedef struct DbPrepared DbPrepared;

struct DbConnection {
    DbConnection_class *oClass;
    lstring *lastError;
    lstring *buffer;
};

struct DbConnection_class {
    void (*destroy)( DbConnection *self );
    int  (*sql_exec)( DbConnection *self, const char *sql, lerror **error );
    DbIterator* (*sql_retrieve)( DbConnection *self, const char *sql, lerror **error );
    DbPrepared* (*sql_prepare)( DbConnection *self, const char *sql, lerror **error );
};

void DbConnection_init( DbConnection *self, DbConnection_class *oClass );
const char *DbConnection_last_error_message( DbConnection *self );

/**
 * Function: DbConnection_destroy
 *
 * Free this database connection
 *
 * Parameters:
 *    self - The database connection. Can be NULL (does nothing)
 */
void DbConnection_destroy( DbConnection *self );

/**
 * Function: DbConnection_sql_exec
 *
 * Execute a query in a DB connection discarding the result.
 *
 * Parameters:
 *    self  - The connection
 *    sql   - The query to execute
 *    error - The error object if you want to know the error message
 *
 * Returns:
 *    A true value if the query run correctly or a false value if an error
 *    was thrown. The error object, if not NULL, is populated with the error
 *    message.
 */
lbool DbConnection_sql_exec( DbConnection *self, const char *sql, lerror **error );

/**
 * Function: DbConnection_sql_retrieve
 *
 * Create an iterator pre-populated with the data selected from the query
 *
 * Parameters:
 *    self  - The connection
 *    sql   - The query to execute
 *    error - The error object if you want to know the error message
 *
 * Returns:
 *    The iterator or NULL in the case of error.
 */
DbIterator *DbConnection_sql_retrieve( DbConnection *self, const char *sql, lerror **error );

/**
 * Function: DbConnection_sql_prepare
 *
 * Create a prepared query. Parameters must be represented by a "?".
 *
 * Parameters:
 *    self  - The connection.
 *    sql   - The query to execute
 *    error - The error object
 *
 * Returns:
 *    The prepared query or NULL in error conditions.
 */
DbPrepared *DbConnection_sql_prepare( DbConnection *self, const char *sql, lerror **error );

/**
 * Function: DbConnection_sql_into
 *
 * Execute a select and get a single result.
 *
 * Parameters:
 *    self  - The connection.
 *    sql   - The query to execute
 *    error - The error object
 *
 * Returns:
 *    The result as a string or NULL. If the result was a NULL you can an empty string.
 *    If there is an error condition this function will return NULL. If the result of the
 *    query doesn't have only one row and one column this function will return an empty
 *    string. In other words, the last condition doesn't represent an error.
 */
const char *DbConnection_sql_into( DbConnection *self, const char *sql, lerror **error );

/**
 * Class: DbIterator
 *
 * This class allow to iterate the results of a query. An instance of this
 * class can be created using the <DbConnection_sql_retrieve> and 
 * <DbPrepared_sql_retrieve> functions.
 *
 * Iterators always starts before the first row so you must call 
 * <DbIterator_prossima_riga> to go to the first row. Ex:
 *
 * (start code)
 * while ( !DbIterator_prossima_riga( iter ) ) {
 *     value = DbIterator_dammi_valore( iter, 0 );
 *     ... something with value ...
 * }
 *
 * DbIterator_destroy( iter );
 * (end)
 */

typedef struct DbIterator_class DbIterator_class;

struct DbIterator_class {
    void (*destroy)( DbIterator *self );
    int  (*dammi_numero_campi)( DbIterator *self );
    const char* (*dammi_nome_campo)( DbIterator *self, int i );
    int (*prossima_riga)( DbIterator *self );
    const char* (*dammi_valore)( DbIterator *self, int i );
    lbool (*controlla_valore_nullo)( DbIterator *self, int i );
};

struct DbIterator {
    DbIterator_class *oClass;
	DbConnection *originatingConnection;
};

void DbIterator_init( DbConnection *connection, DbIterator *self, DbIterator_class *oClass );

/**
 * Function: DbIterator_destroy
 * 
 * Deallocate an iterator
 *
 * Parameters:
 *     self - Iterator (can be NULL)
 */ 
void DbIterator_destroy( DbIterator *self );

/**
 * Function: DbIterator_dammi_numero_campi
 *
 * Returns the number of fields within this iterator
 *
 * Parameters:
 *     self - Iterator (not NULL)
 */
int DbIterator_dammi_numero_campi( DbIterator *self );

/**
 * Function: DbIterator_dammi_nome_campo
 *
 * Returns a field name
 *
 * Parameters:
 *     self - Iterator (not NULL)
 *     i - The column number (starting from 0)
 */
const char *DbIterator_dammi_nome_campo( DbIterator *self, int i );

/**
 * Function: DbIterator_prossima_riga
 *
 * Go to the next row
 *
 * Parameters:
 *     self - Iterator (not NULL)
 *
 * Returns:
 *     TRUE if there was a next row, FALSE elsewhere.
 */
int DbIterator_prossima_riga( DbIterator *self );

/**
 * Function: DbIterator_controlla_valore_nullo
 *
 * Check the iterator to find if the value at the column specified is
 * NULL.
 *
 * Parameters:
 *     self - Iterator (not NULL)
 *     i - The column number (starting from 0)
 *
 * Returns:
 *     TRUE if it was NULL and FALSE elsewhere.
 */
lbool DbIterator_controlla_valore_nullo( DbIterator *self, int i );

/**
 * Function: DbIterator_dammi_valore
 *
 * Get a value from the query result
 *
 * Parameters:
 *     self - Iterator (not NULL)
 *     i - The column number (starting from 0)
 *
 * Returns:
 *     The value in string format. If the result is NULL this function will
 *     return an empty string.
 */
const char *DbIterator_dammi_valore( DbIterator *self, int i );

/**
 * Function: DbIterator_get_originating_connection
 *
 * Get the connection that originated the connection
 *
 * Parameters:
 *   self - The connection
 */
DbConnection *DbIterator_get_originating_connection(DbIterator *self);


/**
 * Class: DbPrepared
 *
 * This class represent a prepared query. You can create a prepared
 * query using <DbConnection_sql_prepare>
 */
typedef struct DbPrepared_class DbPrepared_class;

struct DbPrepared_class {
    void (*destroy)( DbPrepared *self );
    int  (*dammi_numero_parametri)( DbPrepared* self );

    void (*metti_parametro_intero)( DbPrepared* self, int n, int valore );
    void (*metti_parametro_nullo)( DbPrepared* self, int n );
    void (*metti_parametro_stringa)( DbPrepared* self, int n, const char *valore );

    int  (*sql_exec)( DbPrepared *self, lerror **error );
    DbIterator * (*sql_retrieve)( DbPrepared *self, lerror **error );
};

struct DbPrepared {
    DbPrepared_class *oClass;
	DbConnection *originatingConnection;
    lstring *buffer;
    lstring *lastError;
};

void DbPrepared_init( DbConnection *connection, DbPrepared *self, DbPrepared_class *oClass );

/**
 * Function: DbPrepared_destroy
 * Deallocate this prepared query
 *
 * Parameters:
 *     self - The prepared query (can be NULL)
 */
void DbPrepared_destroy( DbPrepared* self );

/**
 * Function: DbPrepared_dammi_numero_parametri
 * Returns the number of parameters in this prepared query
 *
 * Parameters:
 *     self - The prepared query (cannot be NULL)
 */
int DbPrepared_dammi_numero_parametri( DbPrepared* self );

/**
 * Function: DbPrepared_metti_parametro_intero
 * Put an integer parameter in this prepared query
 *
 * Parameters:
 *     self - The prepared query (cannot be NULL)
 *     n - The parameter number (0 <= n < parametersCount)
 *     valore - The value to put
 */
void DbPrepared_metti_parametro_intero( DbPrepared* self, int n, int valore );

/**
 * Function: DbPrepared_metti_parametro_null
 * Put a NULL value in this prepared query
 *
 * Parameters:
 *     self - The prepared query (cannot be NULL)
 *     n - The parameter number (0 <= n < parametersCount)
 */
void DbPrepared_metti_parametro_nullo( DbPrepared* self, int n );

/**
 * Function: DbPrepared_metti_parametro_stringa
 * Put a string parameter in this prepared query
 *
 * Parameters:
 *     self - The prepared query (cannot be NULL)
 *     n - The parameter number (0 <= n < parametersCount)
 *     valore - The value to put
 */
void DbPrepared_metti_parametro_stringa( DbPrepared* self, int n, const char *valore );

/**
 * Function: DbPrepared_sql_exec
 *
 * Executes this prepared query
 *
 * Parameters:
 *   self - The prepared query (not NULL)
 *   error - Eventual space for an error variable
 *
 * Returns:
 *   A true value if all is good and a false one if it's not.
 */
lbool DbPrepared_sql_exec( DbPrepared *self, lerror **error );

/**
 * Function: DbPrepared_sql_retrieve
 *
 * Create an iterator pre-populated with the data selected from the prepared query
 *
 * Parameters:
 *    self  - The prepared query
 *    error - The error object if you want to know the error message
 *
 * Returns:
 *    The iterator or NULL in the case of error.
 */
DbIterator *DbPrepared_sql_retrieve( DbPrepared *self, lerror **error );

/**
 * Function: DbPrepared_sql_into
 *
 * Execute a select and get a single result.
 *
 * Parameters:
 *    self  - The prepared statement
 *    sql   - The query to execute
 *    error - The error object
 *
 * Returns:
 *    The result as a string or NULL. If the result was a NULL you can an empty string.
 *    If there is an error condition this function will return NULL. If the result of the
 *    query doesn't have only one row and one column this function will return an empty
 *    string. In other words, the last condition doesn't represent an error.
 */
const char *DbPrepared_sql_into( DbPrepared *self, lerror **error );

/**
 * Function: DbPrepared_last_error
 *
 * Gives the last error message from the prepared query
 *
 * Parameters:
 *   self - The prepared statement
 *
 * Returns:
 *   The latest error string
 */
const char *DbPrepared_last_error( DbPrepared *self );

/**
 * Function: DbPrepared_get_originating_connection
 *
 * Get the originating connection of this prepared statement
 *
 * Parameters:
 *   self - The prepared statement
 *
 * Returns:
 *   The originating connection
 */
DbConnection *DbPrepared_get_originating_connection(DbPrepared *self);

#endif
