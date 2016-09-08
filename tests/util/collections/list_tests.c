#include "list_tests.h"

#include <assert.h>
#include <string.h>

#include <util/log.h>
#include <util/collections/list.h>

#define MATCH (0)

static void test_init_list()
{
	List* list = init_list();
	assert(list);
	assert(list->size == 0);
	assert(list->head == NULL);
	assert(list->tail == NULL);
	destroy_list(list);
	log_info("PASS\n");
}

static void test_list_append()
{
	List* list = init_list_objects(NULL);

	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_append(list, strdup(key));

		if (i == 0)
			assert(strcmp(list->head->data, key) == MATCH && strcmp((char*) list->tail->data, key) == MATCH);
		else
			assert(strcmp(list->tail->data, key) == MATCH);

		assert(list->size == i + 1);
	}

	destroy_list(list);
	log_info("PASS\n");
}

static void test_list_prepend()
{
	List* list = init_list_objects(NULL);

	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_prepend(list, strdup(key));

		if (i == 0)
			assert(strcmp(list->head->data, key) == MATCH && strcmp((char*) list->tail->data, key) == MATCH);
		else
			assert(strcmp(list->head->data, key) == MATCH);

		assert(list->size == i + 1);
	}

	destroy_list(list);
	log_info("PASS\n");
}

void test_list_get()
{
	List* list = init_list_objects(NULL);

	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_append(list, strdup(key));
	}

	for (unsigned i = 1023; i != 0; i--) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);

		assert(strcmp(list_get(list, i), key) == MATCH);
	}

	destroy_list(list);
	log_info("PASS\n");
}

void test_list_remove()
{
	List* list = init_list_objects(NULL);

	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_append(list, strdup(key));
	}

	for (unsigned i = 1023; i != 0; i--) {
		list_remove(list, i);

		assert(list->size == i);

		if (list->size == 0)
			assert(!list->head && !list->tail);
	}

	destroy_list(list);
	log_info("PASS\n");
}

void list_tests()
{
	test_init_list();
	test_list_append();
	test_list_prepend();
	test_list_get();
	test_list_remove();
}
