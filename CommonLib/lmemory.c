#include "lmemory.h"

#ifdef WIN32
// Questo tipo di allocazione di memoria serve per
// compatibilita' con quella fatta dalla MilleApi, anche se
// non ha cambiato nulla nel debug che stiamo facendo.

// Forse e' il caso di rimettere la malloc normale?

#include <Windows.h>

void *lmalloc(size_t size) {
	return CoTaskMemAlloc(size);
}

void lfree(void *area) {
	CoTaskMemFree(area);
}

void *lrealloc(void *area, size_t newSize) {
	return CoTaskMemRealloc(area, newSize);
}

#endif