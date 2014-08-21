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
#ifndef __BUFFER_H
#define __BUFFER_H

#include <stddef.h>

/**
 * Class: MemBuffer
 *
 * This class represent a simple memory buffer.
 */
typedef struct MemBuffer MemBuffer;

/**
 * Function: MemBuffer_new
 *
 * Create a new memory buffer
 *
 * Parameters:
 *     reservedSpace - The initial reserved space
 */
MemBuffer*      MemBuffer_new( int reservedSpace );

/**
 * Function: MemBuffer_new_fromfile
 *
 * Create a new memory buffer with the contents of a file
 *
 * Parameters:
 *     fileName - The file to read
 *
 * Returns:
 *     A new memory buffer or NULL if the file cannot be read. 
 */
MemBuffer*      MemBuffer_new_fromfile( const char *fileName );

/**
 * Function: MemBuffer_len
 *
 * Get the current length of this memory buffer
 *
 * Parameters:
 *     self - The memory buffer (cannot be NULL)
 */
int             MemBuffer_len( MemBuffer* self );

/**
 * Function: MemBuffer_address
 *
 * Get the memory buffer starting address
 *
 * Parameters:
 *     self - The memory buffer (cannot be NULL)
 */
char *          MemBuffer_address( MemBuffer* self );

/**
 * Function: MemBuffer_address
 *
 * Get the memory buffer space reserve
 *
 * Parameters:
 *     self - The memory buffer (cannot be NULL)
 */
int             MemBuffer_reserve( MemBuffer* self );

/**
 * Function: MemBuffer_write
 * 
 * Append some bytes to this memory buffer
 *
 * Parameters:
 *     self - The memory buffer (cannot be NULL)
 *     addr - The starting address (not NULL)
 *     size - How many bytes to append
 */
void            MemBuffer_write( MemBuffer* self, void *addr, size_t size );

/**
 * Function: MemBuffer_write_char
 *
 * Convenience function to append a byte to the memory buffer
 *
 * Parameters:
 *     self - The memory buffer (not NULL)
 *     c - The byte to append
 */
void            MemBuffer_write_char( MemBuffer* self, char c );

/**
 * Function: MemBuffer_setlen
 *
 * Force the memory buffer to have the passed len. Useful when
 * you want to truncate the memory buffer at a position.
 *
 * Parameters:
 *     self - The memory buffer (not NULL)
 *     len - The size in bytes
 */
void            MemBuffer_setlen( MemBuffer* self, int len );

/**
 * Function: MemBuffer_destroy
 *
 * Deallocate this memory buffer
 *
 * Parameters:
 *     self - The memory buffer to deallocate (can be NULL)
 */
void            MemBuffer_destroy( MemBuffer* self );

#endif
