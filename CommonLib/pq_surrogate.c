#include "pq_surrogate.h"
#include "ldylib.h"

static dylib_handle_t pqlib_handle = NULL;
static void (*pqclear_addr)(PGresult* res) = NULL;
static int (*pqnfields_addr)(PGresult* res) = NULL;
static int (*pqntuples_addr)(PGresult* res) = NULL;
static const char *(*pqfname_addr)(PGresult* res, int i) = NULL;
static int (*pqgetisnull_addr)(PGresult* res, int i, int j) = NULL;
static const char *(*pqgetvalue_addr)(PGresult* res, int i, int j) = NULL;
static PGresult* (*pqexec_addr)(PGconn* connection, const char *sql) = NULL;
static const char *(*pqresulterrormessage_addr)(PGresult* res) = NULL;
static const char *(*pqerrormessage_addr)(PGresult* res) = NULL;
static int (*pqresultstatus_addr)(PGresult* res) = NULL;
static void (*pqfinish_addr)(PGconn* connection) = NULL;
static PGconn* (*pqconnectdb_addr)(const char *connection_string) = NULL;
static int (*pqstatus_addr)(PGconn* connection) = NULL;
static PGresult* (*pqprepare_addr)(PGconn* connection, const char *statementName, const char *query, int nParams, const Oid *paramTypes);
static PGresult* (*pqexecprepared_addr)(PGconn* conn, const char *statementName, int nParams, const char * const *paramValues, const int *paramLenghts, const int *paramFormats, int resultFormat);

void pqsurrogate_init(lerror **error) {
    lerror *myError = NULL;

    if (pqlib_handle!=NULL) return;

#ifdef _WIN32
    pqlib_handle = ldylib_load("libpq.dll", &myError);
#else
    pqlib_handle = ldylib_load("libpq.so", &myError);
#endif /* _WIN32 */
    if (lerror_propagate(error, myError)) return;

    pqclear_addr = ldylib_get_sym(pqlib_handle, "PQclear", &myError);
    if (lerror_propagate(error, myError)) return;

    pqnfields_addr = ldylib_get_sym(pqlib_handle, "PQnfields", &myError);
    if (lerror_propagate(error, myError)) return;

    pqntuples_addr = ldylib_get_sym(pqlib_handle, "PQntuples", &myError);
    if (lerror_propagate(error, myError)) return;

    pqfname_addr = ldylib_get_sym(pqlib_handle, "PQfname", &myError);
    if (lerror_propagate(error, myError)) return;

    pqgetisnull_addr = ldylib_get_sym(pqlib_handle, "PQgetisnull", &myError);
    if (lerror_propagate(error, myError)) return;

    pqgetvalue_addr = ldylib_get_sym(pqlib_handle, "PQgetvalue", &myError);
    if (lerror_propagate(error, myError)) return;

    pqexec_addr = ldylib_get_sym(pqlib_handle, "PQexec", &myError);
    if (lerror_propagate(error, myError)) return;

    pqresulterrormessage_addr = ldylib_get_sym(pqlib_handle, "PQresultErrorMessage", &myError);
    if (lerror_propagate(error, myError)) return;

    pqerrormessage_addr = ldylib_get_sym(pqlib_handle, "PQerrorMessage", &myError);
    if (lerror_propagate(error, myError)) return;

    pqresultstatus_addr = ldylib_get_sym(pqlib_handle, "PQresultStatus", &myError);
    if (lerror_propagate(error, myError)) return;

    pqfinish_addr = ldylib_get_sym(pqlib_handle, "PQfinish", &myError);
    if (lerror_propagate(error, myError)) return;

    pqconnectdb_addr = ldylib_get_sym(pqlib_handle, "PQconnectdb", &myError);
    if (lerror_propagate(error, myError)) return;

    pqstatus_addr = ldylib_get_sym(pqlib_handle, "PQstatus", &myError);
    if (lerror_propagate(error, myError)) return;

    pqprepare_addr = ldylib_get_sym(pqlib_handle, "PQprepare", &myError);
    if (lerror_propagate(error, myError)) return;

    pqexecprepared_addr = ldylib_get_sym(pqlib_handle, "PQexecPrepared", &myError);
    if (lerror_propagate(error, myError)) return;
}

void PQclear(PGresult *res) {
    pqclear_addr(res);
}

int PQnfields(PGresult *res) {
    return pqnfields_addr(res);
}

int PQntuples(PGresult *res) {
    return pqntuples_addr(res);
}

const char *PQfname(PGresult * res, int i) {
    return pqfname_addr(res, i);
}

int PQgetisnull(PGresult * res, int i, int j) {
    return pqgetisnull_addr(res, i, j);
}

const char *PQgetvalue(PGresult * res, int i, int j) {
    return pqgetvalue_addr(res, i, j);
}

PGresult * PQexec(PGconn * conn, const char *sql) {
    return pqexec_addr(conn, sql);
}

const char *PQresultErrorMessage(PGresult * res) {
    return pqresulterrormessage_addr(res);
}

const char *PQerrorMessage(PGconn * conn) {
    return pqerrormessage_addr(conn);
}

int PQresultStatus(PGresult * res) {
    return pqresultstatus_addr(res);
}

void PQfinish(PGconn * conn) {
    pqfinish_addr(conn);
}

PGconn * PQconnectdb(const char *conn_string) {
    return pqconnectdb_addr(conn_string);
}

int PQstatus(PGconn * conn) {
    return pqstatus_addr(conn);
}

PGresult *PQprepare(PGconn *conn,
                    const char *stmtName,
                    const char *query,
                    int nParams,
                    const Oid *paramTypes) {
    return pqprepare_addr(conn, stmtName, query, nParams, paramTypes);
}

PGresult *PQexecPrepared(PGconn *conn,
                         const char *stmtName,
                         int nParams,
                         const char * const *paramValues,
                         const int *paramLengths,
                         const int *paramFormats,
                         int resultFormat) {
    return pqexecprepared_addr(conn, stmtName, nParams, paramValues, paramLengths, paramFormats, resultFormat);
}
