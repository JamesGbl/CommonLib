#ifndef __COMMONLIB_PQ_SURROGATE
#define __COMMONLIB_PQ_SURROGATE

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

#include "lerror.h"

typedef void PGresult;
typedef void PGconn;
typedef void Oid;

#define CONNECTION_OK       0
#define CONNECTION_BAD      1

#define PGRES_COMMAND_OK    1
#define PGRES_TUPLES_OK     2

/**
 * Function: pqsurrogate_init
 * Dynamically load the native postgresql client and use it to implement
 * the surrogate libpq-like functions.
 * This function must be called before every libpq function to initialize
 * the library.
 */
void pqsurrogate_init(lerror **error);

void PQclear(PGresult * res);
int PQnfields(PGresult * res);
int PQntuples(PGresult * res);
const char *PQfname(PGresult * res, int i);
int PQgetisnull(PGresult * res, int i, int j);
const char *PQgetvalue(PGresult * res, int i, int j);
PGresult * PQexec(PGconn * conn, const char *sql);
const char *PQresultErrorMessage(PGresult * res);
const char *PQerrorMessage(PGconn * conn);
int PQresultStatus(PGresult * res);
void PQfinish(PGconn * conn);
PGconn * PQconnectdb(const char *conn_string);
int PQstatus(PGconn * conn); 
PGresult *PQprepare(PGconn *conn,
                    const char *stmtName,
                    const char *query,
                    int nParams,
                    const Oid *paramTypes);
PGresult *PQexecPrepared(PGconn *conn,
                         const char *stmtName,
                         int nParams,
                         const char * const *paramValues,
                         const int *paramLengths,
                         const int *paramFormats,
                         int resultFormat);
#endif
