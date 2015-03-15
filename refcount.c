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

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
int tests_run;


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
