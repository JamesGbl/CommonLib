#ifndef __LCROSS_H
#define __LCROSS_H

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

#include <stdint.h>
#include <stdarg.h>

typedef int lbool;
#define LFALSE (0)
#define LTRUE (1)
#define LNULL (0)

/**
 * Function: l_stricmp 
 * Ignore case compare two strings
 *
 * Parameters:
 *     s1 - First string (not NULL)
 *     s2 - Second string (not NULL)
 *
 * Returns:
 *     As strcmp
 */
int l_stricmp( const char *s1, const char *s2 );

/**
 * Function: l_strdup
 * Cross-compiler version of strdup
 */
char *l_strdup(const char *d);

/**
 * Function: l_strcpy
 * Cross-compiler version of strcpy
 */
void l_strcpy(char *d, const char *s);

/**
 * Function: l_strcat
 * Cross-compiler version of strcat
 */
void l_strcat(char *d, const char *s);

/**
 * Function: l_vsnprintf
 * Cross-compiler version of vsnprintf
 */
void l_vsnprintf(char *d, int len, const char *format, va_list args);

/**
 * Function: l_stricmp
 * Cross-compiler version of stricmp
 */
int l_stricmp(const char *s, const char *m);

/**
 * Function: l_log2
 * Integral base-2 logarithm
 */
uint16_t l_log2(uint32_t n);

/**
 * Function: l_atoi
 * Null-safe version of the atoi function that returns
 * 0 on atoi(NULL)
 */
int l_atoi(const char *s);

#define stringize_op1( x )              #x

/**
 * Macro: stringize_op
 * Magical pre-processor trick to stringize the parameter
 */
#define stringize_op( x ) \
    stringize_op1(x)

/**
 * Macro: l_assert
 * Assertion check. This check doesn't remain in the production code
 */
#ifndef __MSC__
#ifdef DEBUG
#define l_assert(cond) \
    l_assert_internal( cond, __FUNCTION__, __FILE__, __LINE__ )
#else
#define l_assert(cond) {}
#endif
#else
#define l_assert(cond) \
    l_assert_internal( cond, __func__, __FILE__, __LINE__ )
#endif 

void l_assert_internal( lbool condition, const char *function, const char *fileName, int lineNo );

#endif
