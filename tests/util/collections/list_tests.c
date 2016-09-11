#include "list_tests.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <util/log.h>
#include <util/mem_utils.h>
#include <util/collections/list.h>

#include "helper.h"

static List* setup(void (*destructor)(void*))
{
	if (destructor)
		return init_list_objects(destructor);
	else
		return init_list();
}

static void test_init_list()
{
	// Test proper destructor was assigned with list of objects
	List* list = init_list_objects(dummy_destructor);
	assert(list);
	assert(list->destructor == dummy_destructor);
	assert(list->head == NULL);
	assert(list->tail == NULL);
	assert(list->size == 0);
	destroy_list(list);
	
	log_info("PASS\n");
}

static void test_list_append()
{
	List* list = setup(NULL);
	
	// Test with primitive types ------------------------------------------
	int EXPECTED[6] = { 
		rand(), rand(), rand(), rand(), rand(), rand()
	};
	
	void* prev;
	
	for (size_t i = 0; i < sizeof(EXPECTED) / sizeof(int); i++) {
		prev = list->tail;
		list_append(list, &EXPECTED[i]);
		assert(list->size == i+1);
		
		// Check that both head and tail were assigned on first append
		if (list) {
			if (list->size == 1) {
				assert(list->head && list->tail && (list->head == list->tail));
			} else {
				assert(list->tail != prev);
				assert(*(int*) list->tail->data == EXPECTED[i]);
			}
		}
	}
	
	// --------------------------------------------------------------------
	
	destroy(list);
	log_info("PASS\n");
}

static void test_list_prepend()
{
	List* list = init_list_objects(NULL);

	destroy_list(list);
	log_info("PASS\n");
}

void test_list_get()
{
	List* list = init_list_objects(NULL);

	destroy_list(list);
	log_info("PASS\n");
}

void test_list_remove()
{
	List* list = init_list_objects(NULL);

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
