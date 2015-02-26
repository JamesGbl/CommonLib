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

#include "smatrix.h"
#include "lmemory.h"
#include "lstring.h"
#include "lcross.h"

struct smatrix {
	int rows;
	int cols;
	lstring **data;
};

smatrix *smatrix_new(int rowsize, int colsize) {
	smatrix *result = NULL;
	int i;

	l_assert(rowsize>0);
	l_assert(colsize>0);

	result = (smatrix *)lmalloc(sizeof(struct smatrix));
	result->cols = colsize;
	result->rows = rowsize;
	result->data = (lstring **)lmalloc(sizeof(lstring *)*rowsize*colsize);

	for(i=0; i<(rowsize*colsize); i++) {
		result->data[i] = NULL;
	}

	return result;
}

int smatrix_get_rows(smatrix *self) {
	l_assert(self!=NULL);
	return self->rows;
}

int smatrix_get_cols(smatrix *self) {
	l_assert(self!=NULL);
	return self->cols;
}

void smatrix_set(smatrix *self, int row, int col, const char *contents) {
	int idx;

	l_assert(self!=NULL);
	l_assert(row>=0 && row<self->rows);
	l_assert(col>=0 && col<self->cols);
	l_assert(contents!=NULL);

	idx = row*self->cols + col;

	if ( self->data[idx]==NULL ) {
		self->data[idx] = lstring_new_from_cstr(contents);
	} else {
		self->data[idx] = lstring_from_cstr_f(self->data[idx], contents);
	}
}

const char *smatrix_get(smatrix *self, int row, int col) {
	int idx;

	l_assert(self!=NULL);
	l_assert(row>=0 && row<self->rows);
	l_assert(col>=0 && col<self->cols);

	idx = row*self->cols + col;

	if ( self->data[idx]==NULL ) {
		return "";
	} else {
		return self->data[idx];
	}
}

void smatrix_destroy(smatrix *self) {
	int i;

	if(self==NULL) return;

	for(i=0; i<(self->rows*self->cols); i++) {
		if (self->data[i]) {
			lstring_delete(self->data[i]);
		}
	}
}
