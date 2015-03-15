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
#ifndef __REFCOUNT_H
#define __REFCOUNT_H

/**
 * Type: destructor_t
 * This type is respected by destructor functions
 */
typedef void (*destructor_t)(void *object);

/**
 * Function: rc_malloc
 * This function allocates a reference counted memory space.
 * Parameters:
 *   size - The size, in bytes, to allocate
 *   destructor - The destructor that will be called when the
 *    reference count reaches 0. Can be NULL.
 * Returns:
 *   A reference counted memory space with the reference
 *   count initialized at 1.
 */
void *rc_malloc(int size, destructor_t destructor);

/**
 * Function: rc_ref
 * Increments the reference count of this memory space
 * Parameters:
 *   rc - The memory space
 */
void rc_ref(void *rc);

/**
 * Function rc_unref
 * Descrements the reference count of this memory space and,
 * if the reference count reaches 0, invoke the destructor.
 * Parameters:
 *   rc - The memory space
 */
void rc_unref(void *rc);

/**
 * Function: rc_count
 * Gets the current value of the reference count of this
 * memory space.
 * Parameters:
 *   rc - The memory space
 * Returns:
 *   The reference count
 */
int rc_count(void *rc);

#endif
