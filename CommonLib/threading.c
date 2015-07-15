#include "threading.h"
#include "lcross.h"
#include "lmemory.h"
#include <stdio.h>

#ifndef _WIN32
#include <pthread.h>
#else
#include <windows.h>
#endif

#ifdef _WIN32
struct lcom_mutex {
	CRITICAL_SECTION critical;
};

lcom_mutex_t *lcom_mutex_new(void) {
	lcom_mutex_t *result = (lcom_mutex_t *)lmalloc(sizeof(struct lcom_mutex));
	InitializeCriticalSection(&result->critical);
	return result;
}

void lcom_mutex_destroy(lcom_mutex_t *mutex) {
	l_assert(mutex!=NULL);
	DeleteCriticalSection(&mutex->critical);
}

void lcom_mutex_lock(lcom_mutex_t *mutex) {
	l_assert(mutex!=NULL);
	EnterCriticalSection(&mutex->critical);
}

void lcom_mutex_unlock(lcom_mutex_t *mutex) {
	l_assert(mutex!=NULL);
	LeaveCriticalSection(&mutex->critical);
}
#else

#endif
