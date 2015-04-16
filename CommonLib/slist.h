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

/*
File: slist.h
*/

#ifndef __SLIST_H
#define __SLIST_H

/**
 * Class: slist
 *
 * This is a simple database that implements a list of strings.
 */
typedef struct slist slist;

/**
 * Function: slist_new
 *
 * Create a new list. All string elements are created empty
 *
 * Parameters:
 *     initialSize - The initial size of this list.
 */
slist*         slist_new( int initialSize );

/**
 * Function: slist_new_copy
 * Create a new slist from an existing slist
 * Parameters:
 *   other - The other slist
 * Returns:
 *   An slist
 */
slist *slist_new_copy(const slist *other);

/**
 * Function:  slist_destroy
 *
 * Destroy the list and deallocate all strings
 *
 * Parameters:
 *     self - The list (can be NULL)
 */
void           slist_destroy( slist* self );

/**
 * Function: slist_resize
 *
 * Resize the list.
 *
 * Parameters:
 *     self - The list (cannot be NULL)
 *     newLen - The new length of the list
 */ 
void           slist_resize( slist* self, int newLen );

/**
 * Function: slist_len
 *
 * Return the current length of the list
 *
 * Parameters:
 *     self - The list (cannot be NULL)
 */
int            slist_len( slist* self );

/**
 * Function: slist_at
 *
 * Return the string at the specified position
 *
 * Parameters:
 *     self - The list (cannot be NULL)
 *     n - The index of the element (0<=n<len)
 */
const char*    slist_at( slist *self, int n );

/**
 * Function: slist_set
 *
 * Put a copy of the specified string in the list
 *
 * Parameters:
 *     self - The list (cannot be NULL)
 *     n - The index of the element (0<=n<len)
 *     str - The string (cannot be NULL)
 */
void           slist_set( slist *self, int n, const char *str );

#endif
