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
#include "lmemory.h"
#include <string.h>

#ifdef LMEM_USE_COTASK_ALLOCATOR
// Questo tipo di allocazione di memoria serve per
// compatibilita' con quella fatta dalla MilleApi, anche se
// non ha cambiato nulla nel debug che stiamo facendo.

// Forse e' il caso di rimettere la malloc normale?

#include <Windows.h>

void *lmalloc(size_t size) {
	if (size==0) return NULL;
	return CoTaskMemAlloc(size);
}

void lfree(void *area) {
	CoTaskMemFree(area);
}

void *lrealloc(void *area, size_t newSize) {
	return CoTaskMemRealloc(area, newSize);
}

#else

#include <stdlib.h>

void *lmalloc(size_t size) {
	if (size==0) return NULL;
	return malloc(size);
}

void lfree(void *area) {
	free(area);
}

void *lrealloc(void *area, size_t newSize) {
	return realloc(area, newSize);
}

#endif

void *lmalloczero(size_t size) {
	void *result;

	if (size==0) return NULL;
	result = lmalloc(size);
	if(result!=NULL) {
		memset(result, 0, size);
	}
	return result;
}
