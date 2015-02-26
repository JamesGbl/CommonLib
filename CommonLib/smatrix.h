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
#ifndef __SMATRIX_H
#define __SMATRIX_H

typedef struct smatrix smatrix;

/**
 * Creates a new string matrix data type.
 * 
 * Parameters:
 *   rowsize - How many rows the matrix should have (must be greeter than zero)
 *   colsize - How many columns the matrix should have (must be greeter than zero)
 */
smatrix *smatrix_new(int rowsize, int colsize);

/**
 * Returns the number of rows of this string matrix
 *
 * Parameters:
 *   self - The string matrix (should not be null)
 */
int smatrix_get_rows(smatrix *self);

/**
 * Returns the number of columns of this string matrix
 *
 * Parameters:
 *   self - The string matrix (should not be null)
 */
int smatrix_get_cols(smatrix *self);

/**
 * Write a string to a cell
 *
 * Parameters:
 *   self - The string matrix (should not be null)
 *   row - The number of rows (indexes begins with 0)
 *   cols - The number of columns (indexes begins with 0)
 *   contents - The string contents (should be not null)
 */
void smatrix_set(smatrix *self, int row, int col, const char *contents);

/**
 * Get the contents of a matrix cell
 *
 * Parameters:
 *   self - The string matrix (should not be null)
 *   row - The number of rows (indexes begins with 0)
 *   cols - The number of columns (indexes begins with 0)
 */
const char *smatrix_get(smatrix *self, int row, int col);

/**
 * Destroy a matrix and frees all the memory cells
 *
 * Parameters:
 *   self - The string matrix (can be null)
 */
void smatrix_destroy(smatrix *self);

#endif