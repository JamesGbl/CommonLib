#ifndef __LMEMORY_H
#define __LMEMORY_H

#include <stdlib.h>

/**
 * Function: lmalloc
 * Alloca un blocco di memoria rimandando all'allocatore di sistema
 */
void *lmalloc(size_t size);

/**
 * Function: lfree
 * Dealloca un'area passata
 */
void lfree(void *area);

/**
 * Function: lrealloc
 * Realloca un'area di memoria
 */
void *lrealloc(void *area, size_t newSize);

#endif