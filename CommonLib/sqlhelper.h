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

#ifndef __SQLHELPER_H
#define __SQLHELPER_H

/**
 * Class: SqlHelper
 *
 * Objects of this class will help you to generate an INSERT statement
 * with a variable number of fields (and values with parameters)
 */
typedef struct SqlHelper SqlHelper;

/**
 * Function: SqlHelper_init
 *
 * Create a new SqlHelper structure
 *
 * Parameters:
 *   nomeTabella - The table name
 *
 * Returns:
 *   A new SqlHelper structure
 */
SqlHelper* SqlHelper_init( const char *nomeTabella );

/**
 * Function: SqlHelper_aggiungi_campo
 *
 * Add a field
 *
 * Parameters:
 *   self - The object (must be not NULL)
 *   nomeCampo - The field name
 */
void SqlHelper_aggiungi_campo( SqlHelper *self, const char *nomeCampo );

/**
 * Function: SqlHelper_to_insert
 *
 * Create a new INSERT statement
 *
 * Parameters:
 *   self - The object (must be not NULL)
 */
const char *SqlHelper_to_insert( SqlHelper *self );

/**
 * Function: SqlHelper_destroy
 *
 * Destroy this object
 *
 * Parameters:
 *   self - The object to destroy (can be NULL)
 */
void SqlHelper_destroy( SqlHelper *self );

#endif
