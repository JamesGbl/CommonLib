#ifndef __LERROR_H_
#define __LERROR_H_

#include "lstring.h"
#include "lcross.h"

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

typedef struct lerror lerror;
struct lerror {
    char *message;
    char *domain;

    lstring *stacktrace;
};

#ifdef _MSC_VER
#define lerror_mark( error )            lerror_add_stack_internal( error, __FUNCTION__, "Called from: " __FILE__ " at " stringize_op( __LINE__ ) )
#else
#define lerror_mark( error )            lerror_add_stack_internal( error, __func__, "Called from: " __FILE__ " at " stringize_op( __LINE__ ) )
#endif

/**
 * Macro: lerror_set
 *
 * Populate an error status variable
 *
 * Parameters:
 *   err - The error status variable to populate. Can be NULL but, if err!=NULL, then
 *         *err must be NULL. You can not invoke this function on a populated error
 *         variable.
 *   domain - The domain of the error (syslog like)
 *   message - The error message. Must not be NULL.
 *
 */
#ifdef _MSC_VER
#define lerror_set(err, message ) \
    do { lerror_set_internal(err, __FUNCTION__, message); lerror_mark(err); } while (0)
#else
#define lerror_set(err, message ) \
    do { lerror_set_internal(err, __func__, message); lerror_mark(err); } while (0)
#endif

/**
 * Macro: lerror_add_prefix
 *
 * Adds a prefix to a error variable
 * 
 * Parameters:
 *   err - The error variable. Must not be NULL and *err must not
 *         be NULL.
 *   prefix - The prefix to add. Must not be NULL.
 */
#define lerror_add_prefix(err, prefix) \
    do { lerror_mark(err); lerror_add_prefix_internal(err, prefix); } while(0)

/**
 * Function: lerror_set_sprintf
 *
 * Populate error status variable with a sprintf-like format.
 *
 * Parameters:
 *   err - The error status variable to populate. Can be NULL but, if err!=NULL, then
 *         *err must be NULL. You can not invoke this function on a populated error
 *         variable.
 *   domain - The domain of the error (syslog like)
 *   message - The error message format.
 */
#ifdef _MSC_VER
#define lerror_set_sprintf(error,...) \
    do { lerror_set_sprintf_internal(error, __FUNCTION__, __VA_ARGS__); lerror_mark(error); } while(0)
#else
#define lerror_set_sprintf(error,...) \
    do { lerror_set_sprintf_internal(error, __func__, __VA_ARGS__); lerror_mark(error); } while(0)
#endif

/**
 * Function: lerror_delete
 *
 * Frees an error message. If err==NULL this function is a NOP.
 */
void lerror_delete( lerror **err );

/**
 * Macro: lerror_propagate
 *
 * Propagate the source error to the destination. 
 * If "destination" is NULL free the "source" error.
 * If "destination" is not NULL move the "source" error to the "destination"
 *
 * Parameters:
 *     destination - The error destination (can be NULL)
 *     source - The source error (cannot be NULL)
 */
#ifdef _MSC_VER
#define lerror_propagate(destination, source)				\
    lerror_propagate_internal( destination, source, __FUNCTION__, "Called from: " __FILE__ " at " stringize_op( __LINE__ ) )
#else
#define lerror_propagate(destination, source)				\
    lerror_propagate_internal( destination, source, __func__, "Called from: " __FILE__ " at " stringize_op( __LINE__ ) )
#endif

/**
 * Function: lerror_fill_f
 * Fill the string with informations from the error status
 * Parameters:
 *     error - The error (can't be NULL)
 *     str - The buffer (can't be NULL)
 */
lstring* lerror_fill_f( lerror *error, lstring *str );


lbool lerror_propagate_internal( lerror **destination, lerror *source, const char *domain, const char *location );
void lerror_add_stack_internal( lerror **destination, const char *domain, const char *location );
void lerror_add_prefix_internal( lerror **err, const char *prefix );
void lerror_set_internal( lerror **err, const char *domain, const char *message );
void lerror_set_sprintf_internal( lerror **err, const char *domain, const char *message, ... );

#endif
