#ifndef __DB_UTILS_H
#define __DB_UTILS_H

#include "lstring.h"

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

/**
 * File: db_utils.h
 */

#include "db_interface.h"

/**
 * Function: db_append_sql_escaped
 * Append to a query an escaped string param
 * Parameters:
 *    str - Destination lstring
 *    value - The string to escape (can be NULL)
 */
lstring* db_append_sql_escaped_f( lstring *str, const char *value );

/**
 * Function: db_put_sql_format
 * This function is like db_append_sql_format but will reset the string before
 * writing the query.
 * Parameters:
 *     str - Destination lstring
 *     format - Format string
 */
lstring* db_put_sql_format_f( lstring *str, const char *format, ... );

/**
 * Function: db_append_sql_format
 * This is a convenience function to prepare a SQL query. The format string, which
 * is printf-like, can contain placeholders like:
 *     "%k" to represent a literal string (keyword)
 *     "%m" to represent a meta-data name as a field name of a table name. If the
 *          field name is a SQL keyword it will be quoted in the SQL query
 *     "%i" to represent a integer parameter
 *     "%s" to represent a string parameter
 * Parameters:
 *     str - Destination lstring
 *     format - Format string
 */
lstring* db_append_sql_format_f( lstring *str, const char *format, ... );

/**
 * Function: db_compare_datetime
 * Compare two date/time string with the format "YYYY-MM-DD HH:MM". 
 * The string is compared until there are informations: this
 * means that "2010-01-01" and "2010-01-01 02:00" are considered
 * equal.
 *
 * Parameters:
 *    first - First string to compare (not NULL)
 *    second - Second string to compare (not NULL)
 *
 * Returns:
 *    -1 if first>second, 0 if first==second, 1 if first<second
 */
int db_compare_datetime( const char *first, const char *second );

/**
 * Function: db_execute_sql_script
 *
 * Execute a SQL script splitting it on individual SQL instructions and
 * forwarding them, one by one, to the DBMS.
 *
 * Parameters:
 *    conndb - The connection where to execute the queries
 *    sql_script - The SQL script
 */
void db_execute_sql_script(DbConnection *conndb, const char *sql_script, lerror **error);

#endif
