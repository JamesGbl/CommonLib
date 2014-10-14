#ifndef __TRANSACTION_H
#define __TRANSACTION_H

#include "lcross.h"

typedef void (*destructor_t)(void *res);

#define MAX_RESOURCES_PER_TRANSACTION	64

struct ltransaction {
	void *resources[MAX_RESOURCES_PER_TRANSACTION];
	destructor_t destructors[MAX_RESOURCES_PER_TRANSACTION];
	lbool aborted;
	int count_res;
};
typedef struct ltransaction ltransaction;


/**
 * Function: ltransaction_new
 * Create a new transaction object
 */
ltransaction *ltransaction_new(void);

/**
 * Function: ltransaction_clear
 * Free a transaction object and all its associated
 * resources.
 *
 * Parameters:
 *   trs - The transaction to free
 */
void ltransaction_clear(ltransaction *trs);

/**
 * Function: ltransaction_push
 * Assign a resource to a transaction
 *
 * Parameters:
 *   trs - The transaction
 *   res - The resource owned by the transaction
 *   cbk - The callback function that will be invoked to free the resource
 */
void ltransaction_push(ltransaction *trs, void *res, destructor_t cbk);

/**
 * Function: ltransaction_aborted 
 * Check if this transaction is aborted
 *
 * Parameters:
 *   trs - The transaction
 *   location - To to full by a macro that write here the current
 *      function name and the file/line of the source code
 */
lbool ltransaction_aborted_internal(ltransaction *trs, const char *location);

#ifdef _MSC_VER
#define ltransaction_aborted( trs ) \
  ltransaction_aborted( trs, __FUNCTION__, "Called from: " __FILE__ " at " stringize_op( __LINE__ ) )
#else
#define ltransaction_aborted( trs ) \
  ltransaction_aborted( trs, __func__, "Called from: " __FILE__ " at " stringize_op( __LINE__ ) )
#endif

#endif

