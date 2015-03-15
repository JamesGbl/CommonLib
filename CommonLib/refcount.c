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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "refcount.h"

typedef struct {
	destructor_t destructor;
	int rc;
} rc_descriptor;

void *rc_malloc(size_t size, destructor_t destructor) {
	void *result = NULL;
	rc_descriptor *descriptor;
	
	assert(size>0);
	result = malloc(size+sizeof(rc_descriptor));
	descriptor = result;
	descriptor->destructor = destructor;
	descriptor->rc = 1;
	result = result + sizeof(rc_descriptor);

	return result;
}

void rc_ref(void *rc) {
	rc_descriptor *descriptor;

	descriptor = rc;
	descriptor = descriptor - 1;
	descriptor->rc++;
}

void rc_unref(void *rc) {
	rc_descriptor *descriptor;

	descriptor = rc;
	descriptor = descriptor - 1;
	descriptor->rc--;
	if (0==descriptor->rc) {
		if (descriptor->destructor)
			descriptor->destructor(rc);
		free(descriptor);
	}
}

int rc_count(void *rc) {
	rc_descriptor *descriptor;

	descriptor = rc;
	descriptor = descriptor - 1;
	return descriptor->rc;
}

