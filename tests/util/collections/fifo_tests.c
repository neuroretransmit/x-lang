#include "fifo_tests.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <util/debug.h>
#include <util/collections/fifo.h>

#define MATCH (0)

static void test_init()
{
	FIFO* fifo = init_fifo();
	assert(fifo);
	assert(fifo->size == 0);
	assert(!fifo->head);
	assert(!fifo->tail);
	destroy_fifo(fifo);
	log_info("PASS\n");
}

static void test_push_pop()
{
	FIFO* fifo = init_fifo_objects(NULL);

	for (unsigned i = 0; i < 1024; i++) {
		assert(fifo->size == i);

		char key[100];
		sprintf(key, "i_am_the_key%u", i);
		fifo_push(fifo, key);
	}

	for (unsigned i = 0, j = 1024; i < 1024; i++, j--) {
		assert(fifo->size == j);

		char key[100];
		sprintf(key, "i_am_the_key%u", i);

		assert(strcmp(fifo_pop(fifo), key) == MATCH);
	}

	destroy_fifo(fifo);
	log_info("PASS\n");
}

static void test_fifo_empty()
{
	FIFO* fifo = init_fifo_objects(NULL);
	assert(fifo_empty(fifo));
	fifo_push(fifo, strdup("TEST"));
	assert(!fifo_empty(fifo));
	destroy_fifo(fifo);
}

void fifo_tests()
{
	test_init();
	test_push_pop();
	test_fifo_empty();
}
