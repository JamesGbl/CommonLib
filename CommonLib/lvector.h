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
#ifndef __LVECTOR_C_H
#define __LVECTOR_C_H

struct lvector {
	void **buffer;
	int size;
};

/**
 * Struct: lvector
 * This structure represents a vector of dynamic size
 */
typedef struct lvector lvector;

/**
 * Function: lvector_new
 * Initializes a new vector with an initial defined size
 * Parameters:
 *   size - Initial size
 */
lvector* lvector_new(int size);

/**
 * Function: lvector_new_copy
 * Create a new lvector with the data of the existing one
 * Parameters:
 *   other - The lvector to copy
 */
lvector* lvector_new_copy(const lvector *other);

/**
 * Function: lvector_len
 * Returns the length of the vector
 * Parameters:
 *   self - The vector to operate on
 */
int lvector_len(lvector* self);

/**
 * Function: lvector_at
 * Gets an element from the vector
 * Parameters:
 *   self - The vector to operate on
 *   idx - The index, starting from 0
 * Returns:
 *   The element
 */
void *lvector_at(lvector* self, int idx);

/**
 * Function: lvector_resize
 * Enlarge the vector
 * Parameters:
 *   self - The vector to operate on
 *   newSize - The new size
 */
void lvector_resize(lvector* self, int newSize);

/**
 * Function: lvector_set
 * Put an element inside this vector
 * Parameters:
 *   self - The vector to operate on
 *   idx - The index of the vector element to substitute
 *   element - The element to put on the vector
 */
void lvector_set(lvector* self, int idx, void *element);

/**
 * Function: lvector_delete
 * Deallocates the vector
 * Parameters:
 *   self - The vector to operate on
*/
void lvector_delete(lvector* self);

/**
 * Function: lvector_address
 * Get the starting address of the vector data
 */
void *lvector_address(lvector *self);

#endif
