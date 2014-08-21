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
DbConnection_Sqlite *DbConnection_Sqlite_new( const char *fileName );

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
