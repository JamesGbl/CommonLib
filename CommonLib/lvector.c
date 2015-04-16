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
/* vim: set ai shiftwidth=2 tabstop=2 expandtab: */

#include <malloc.h>
#include <stdio.h>
#include "lvector.h"
#include "lcross.h"
#include "lmemory.h"

lvector* lvector_new(int size)
{
	lvector *self = (lvector *)lmalloc(sizeof(lvector));
	self->buffer = lmalloc( sizeof(void*) * size );
	self->size = size;
	return self;
}

lvector* lvector_new_copy(const lvector *other) {
	int i;
	lvector *self = lvector_new(other->size);

	for(i=0; i<self->size; i++) {
		self->buffer[i] = other->buffer[i];
	}

	return self;
}

int lvector_len(lvector* self)
{
	return self->size;
}

void *lvector_at(lvector* self, int idx)
{
	l_assert( 0<=idx && idx<self->size );
	return self->buffer[idx];
}	

void lvector_resize(lvector* self, int newSize)
{
	if( self->size < newSize )
	{
		self->buffer = lrealloc(self->buffer, sizeof(void *)*newSize);
	} 

	self->size = newSize;
}

void lvector_set(lvector *self, int idx, void *element)
{
	l_assert(0<=idx && idx<self->size);
	self->buffer[idx]=element;
}

void lvector_delete(lvector *self)
{
    if ( self==NULL ) return;
	lfree(self->buffer);
	lfree(self);
}

void *lvector_address(lvector *self) 
{
    if ( self==NULL ) return NULL;
    return self->buffer;
}
