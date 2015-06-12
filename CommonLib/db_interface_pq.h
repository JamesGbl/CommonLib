#ifndef WIN32
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

#ifndef __DB_INTERFACE_PQ_H
#define __DB_INTERFACE_PQ_H

#define POSTGRESQL_CONNECTION_TYPE     "postgresql"

/**
 * File: db_interface_pq.h
 */

#include "db_interface.h"
#include "lerror.h"

/**
 * Function: DbConnection_Pq_new
 *
 * Create a connection to a PostgreSQL database
 *
 * Parameters:
 *     connString - The connection string (dbname=milleps user=dba password=sql host=1.2.3.4)
 *     error - Eventual space for an error variable
 *
 * Returns:
 *     The created connection or NULL if there was some error.
 */
DbConnection *DbConnection_Pq_new( const char *connString, lerror **error );


#endif
#endif
