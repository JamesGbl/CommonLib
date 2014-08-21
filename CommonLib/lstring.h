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

#ifndef __LSTRING_C_H
#define __LSTRING_C_H

/**
 * Class: lstring
 * Yet another fancy string class
 */
typedef struct lstring lstring;

struct lstring
{
	char *buf;
	int len;
	int bufLen;
};

/**
 * Function: lstring_new 
 * Creates an empty string
 */
lstring *lstring_new( void );

/**
 * Function: lstring_new_from_cstr
 * Creates an empty string with the contents of the cstring passed
 *
 * Parameters:
 *     cstr - The cstring to copy (not NULL)
 */
lstring *lstring_new_from_cstr( const char *cstr );

/**
 * Function: lstring_new_from_lstr
 * Creates an empty string with the contents of the lstring passed
 *
 * Parameters:
 *     self - The lstring to copy (not NULL)
 */
lstring *lstring_new_from_lstr( lstring* self );

/**
 * Function: lstring_from_cstr
 * Erase the content of this lstring and populate it with
 * the cstring passed
 *
 * Parameters:
 *     self - The lstring to clear (not NULL)
 *     other - The cstring to copy (not NULL)
 */
void lstring_from_cstr( lstring* str, const char *other );

/**
 * Function: lstring_from_lstr
 * Erase the content of this lstring and populate it with
 * the lstring passed
 *
 * Parameters:
 *     self - The lstring to clear (not NULL)
 *     other - The cstring to copy (not NULL)
 */
void lstring_from_lstr( lstring* str, lstring *other );

/**
 * Function: lstring_delete
 * Delete this lstring
 *
 * Parameters:
 *     str - The lstring to delete (can be NULL)
 */
void lstring_delete( lstring* str );

/**
 * Function: lstring_append_cstr
 * Append a cstring to this lstring
 *
 * Parameters:
 *     str - The lstring (not NULL)
 *     other - The cstring to append (not NULL)
 */
void lstring_append_cstr( lstring* str, const char *other );

/**
 * Function: lstring_append_lstring
 * Append an lstring to this lstring
 *
 * Parameters:
 *     str - The lstring (not NULL)
 *     other - The lstring to append (not NULL)
 */
void lstring_append_lstring( lstring* str, lstring* other );

/**
 * Function: lstring_append_char
 * Append a char to this lstring
 *
 * Parameters:
 *     str - The lstring (not NULL)
 *     c - The char to append
 */
void lstring_append_char( lstring* str, char c );

/**
 * Function: lstring_to_cstr
 * Get the cstring out from this lstring
 *
 * Parameters:
 *     str - The lstring (not NULL)
 */
const char *lstring_to_cstr( lstring* str );

/**
 * Function: lstring_reserve
 * Enlarge the reserved space for this lstring
 *
 * Parameters:
 *     str - The lstring (not NULL)
 *     len - The new reserved space
 */
void lstring_reserve( lstring* str, int len );

/**
 * Function: lstring_ltrim
 * Trim this string from the left
 *
 * Parameters:
 *     str - The lstring (not NULL)
 */
void lstring_ltrim( lstring* str );

/**
 * Function: lstring_rtrim
 * Trim this string from the right
 *
 * Parameters:
 *     str - The lstring (not NULL)
 */
void lstring_rtrim( lstring* str );

/**
 * Function: lstring_trim
 * Trim this string from the right and from the left
 *
 * Parameters:
 *     str - The lstring (not NULL)
 */
void lstring_trim( lstring* str );

/**
 * Function: lstring_contains
 * Check if this lstring contains a string
 *
 * Parameters:
 *     str - The heystack (not NULL)
 *     other - The needle
 *
 * Returns:
 *     TRUE if the needle is in the heystack, FALSE otherwise
 */
int lstring_contains( lstring* str, const char *other );

/*
** Controlla l'uguaglianza di due stringhe, di cui una in stile C
*/
int lstring_equals_cstr( lstring* str, const char *cstr );

/*
** Controlla l'uguaglianza di due stringhe
*/
int lstring_equals_lstr( lstring* str_1, lstring* str_2 );

/**
 * Function: lstring_toupper
 * Convert to upper-case this lstring
 * Parameters:
 *     str - The lstring (not NULL)
 */
void lstring_toupper( lstring* str );

/**
 * Function: lstring_toupper
 * Convert to lower-case this lstring
 * Parameters:
 *     str - The lstring (not NULL)
 */
void lstring_tolower( lstring* str );

/*
** In una certa stringa rimpiazza tutte le occorrenze di un 
** carattere con un'altro
*/
void lstring_replacechar( lstring* str, char orig, char dest );

/*
** Controlla se una certa stringa ha un'altra stringa per suffisso
*/
int lstring_ends_with_cstr( lstring* str, const char *suffix );

/*
** Prende l'ultimo indice di un certo carattere in una stringa. Se il
** carattere non appare restituisce -1
*/
int lstring_last_index_of( lstring* str, char c );

/**
 * Function: lstring_truncate
 * Truncates the lstring to a certain length
 *
 * Parameters:
 *     str - The lstring (not NULL)
 */
void lstring_truncate( lstring* str, int l );

/**
 * Function: lstring_reset
 * Clear the lstring
 *
 * Parameters:
 *     str - The lstring (not NULL)
 */
void lstring_reset( lstring* str );

/**
 * Function: lstring_len
 * Returns the length of a lstring
 *
 * Parameters:
 *     str - The lstring (not NULL)
 */
int lstring_len( lstring* str );

/*
** Dimensione del buffer
*/
int lstring_buffer_size( lstring *str );

/*
** Aggiunge alla stringa
*/
void lstring_append_generic( lstring* str, const char *other, int otherLen);

/*
** Aggiunge alla stringa un preformattato sprintf
*/
void lstring_append_sprintf( lstring *str, const char *format, ... );

#endif
