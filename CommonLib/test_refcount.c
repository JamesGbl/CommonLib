#include "refcount.h"
#include <stdio.h>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
int tests_run;

static const char *test_initial_ref() {
	void *tentativo;

	tentativo = rc_malloc(1024, NULL);
	mu_assert("allocation successful", tentativo!=0);
	mu_assert("initial reference count", rc_count(tentativo)==1);
	rc_unref(tentativo);

	return 0;
}

static const char *test_ref() {
	void *tentativo;

	tentativo = rc_malloc(1024, NULL);
	mu_assert("allocation successful", tentativo!=0);
	mu_assert("initial reference count", rc_count(tentativo)==1);
	rc_ref(tentativo);
	mu_assert("reference count was updated after ref", rc_count(tentativo)==2);
	rc_unref(tentativo);
	mu_assert("reference count was updated after unref", rc_count(tentativo)==1);
	rc_unref(tentativo);

	return 0;
}

struct fiction_object {
	int *o;
};

static void destructor_test_1(struct fiction_object *d) {
	*(d->o) = 0;
}

static const char *test_launch_destructor() {
	struct fiction_object *tentativo;
	int flag;
	
	tentativo = (struct fiction_object *) rc_malloc(sizeof(struct fiction_object), (destructor_t)destructor_test_1);
	tentativo->o = &flag;
	mu_assert("allocation successful", tentativo!=0);
	mu_assert("initial reference count", rc_count(tentativo)==1);

	flag = 1;
	mu_assert("initial flag", flag==1);
	rc_unref(tentativo);
	mu_assert("final flag", flag==0);

	return 0;
}

static const char *all_tests() {
	mu_run_test(test_initial_ref);
	mu_run_test(test_ref);
	mu_run_test(test_launch_destructor);
	return 0;
}

int main() {
	const char *result = all_tests();
	if (result) {
		printf("Test errato: %s\n", result);
	} else {
		printf("OK. Ho eseguito %i tests\n", tests_run);
	}

	return 0;
}
