#ifndef __COMMONLIB_DB_INTERFACE_LOGGING_H
#define __COMMONLIB_DB_INTERFACE_LOGGING_H

#include "db_interface.h"

/**
 * Function: DbConnection_create_logging
 * Create a dataconnection decorators that logs every query using CommonLib
 * logging
 *
 * Parameters:
 *    parent - The parent data connection, that must not be NULL
 * Returns:
 *    The logging data connection
 */
DbConnection *DbConnection_create_logging(DbConnection *parent);

#endif
