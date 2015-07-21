#ifndef __COMMONLIB_THREADING_H
#define __COMMONLIB_THREADING_H

/**
 * Type: lcom_mutex_t
 * This type represent a critical section
 */
typedef struct lcom_mutex lcom_mutex_t;

/**
 * Function: lcom_mutex_init
 * Initialize this critical section
 * Returns: a new critical section
 */
lcom_mutex_t *lcom_mutex_new(void);

/**
 * Function: lcom_mutex_destroy
 * Destroy the resources associated to this critical section
 * Parameters:
 *   mutex - The mutex
 */
void lcom_mutex_destroy(lcom_mutex_t *mutex);

/**
 * Function: lcom_mutex_lock
 * Lock this critical section, other threads cannot enter
 * Parameters:
 *   mutex - The mutex
 */
void lcom_mutex_lock(lcom_mutex_t *mutex);

/**
 * Function: lcom_mutex_unlock
 * Unlock this critical section, other threads can enter
 * Parameters:
 *   mutex - The mutex
 */
void lcom_mutex_unlock(lcom_mutex_t *mutex);

#endif
