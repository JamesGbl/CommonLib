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

Author: Leonardo Cecchi <leonardoce@interfree.it>
*/ 

#ifndef __DB_INTERFACE_SQLITE_H
#define __DB_INTERFACE_SQLITE_H

#include "db_interface.h"

#define SQLITE_CONNECTION_TYPE "sqlite"

typedef struct DbConnection_Sqlite DbConnection_Sqlite;

/**
 * Function: DbConnection_Sqlite_new
 *
 * Create a connection to a SQLite file
 *
 * Parameters:
 *     fileName - The name of the database
 * 
 * Returns:
 *     The connection or NULL in error conditions (IO error)
 */
DbConnection_Sqlite *DbConnection_Sqlite_new( const char *fileName, lerror **error );

/**
 * Function: DbConnection_Sqlite_new_mem
 *
 * Create a connection to a SQLite memory database
 *
 * Returns:
 *     The connection
 */
DbConnection_Sqlite *DbConnection_Sqlite_new_mem( void );

/**
 * Function: DbConnection_Sqlite_new_mem_shared
 *
 * Create a connection to a SQLite memory database with a shared
 * cache. With this function you can create multiple connections 
 * to the same memory database which in not discarded until
 * there is an open connection.
 *
 * Parameters:
 *     dbname - The name of the memory database
 *
 * Returns:
 *     The connection
 */
DbConnection_Sqlite *DbConnection_Sqlite_new_mem_shared(const char *dbname);

/**
 * Function: DbConnection_Sqlite_new_embed
 *
 * Embed an existing sqlite3* connection in this high-lever connection.
 * The low-level connection will not be destroyed when the high level
 * connection will be.
 *
 * Returns:
 *     The connection
 */
DbConnection_Sqlite *DbConnection_Sqlite_new_embed(void *handle);

/**
 * Function: DbConnection_Sqlite_get_handle
 *
 * Get the internal SQLite handle of this data connection. This function
 * is useful if you want to use some SQLite related functions that the
 * database independent API doesn't offer, ad ex. adding custom C
 * functions to the SQLite SQL language.
 *
 * Parameters:
 *   conn - The SQLite data connection (must not be NULL)
 *
 * Returns:
 *   The internal SQLite connection (sqlite3 *)
 */
void *DbConnection_Sqlite_get_handle(DbConnection_Sqlite *conn);

/**
 * Function: DbConnection_export_to_sqlite
 * 
 * Save a SQLite database to a file
 * 
 * Parameters:
 *     self - The connection
 *     fileName - The destination file
 *     error - Space for an error variable
 * 
 * Returns:
 *     True if all went OK and false otherwise.
 */
lbool DbConnection_export_to_sqlite( DbConnection_Sqlite *self, const char *fileName, lerror **error );

#endif
