#include "list_tests.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <util/log.h>
#include <util/mem_utils.h>
#include <util/collections/list.h>

#include "helper.h"

static List* setup(void (*destructor)(void*))
{
	srand(time(NULL));
	
	if (destructor)
		return init_list_objects(destructor);
	else
		return init_list_objects(NULL);
}

static void init_list_test()
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

static void list_append_test()
{
	List* list = setup(NULL);
	
	int EXPECTED[6] = {  rand(), rand(), rand(), rand(), rand(), rand() };
	
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
	
	destroy_list(list);
	log_info("PASS\n");
}

static void list_prepend_test()
{
	List* list = setup(NULL);
	
	int EXPECTED[6] = {  rand(), rand(), rand(), rand(), rand(), rand() };
	
	void* prev;
	
	for (size_t i = 0; i < 6; i++) {
		prev = list->head;
		list_prepend(list, &EXPECTED[i]);
		assert(list->size == i+1);
		
		// Check that both head and tail were assigned on first append
		if (list) {
			if (list->size == 1) {
				assert(list->head && list->tail && (list->head == list->tail));
			} else {
				assert(list->head != prev);
				assert(*(int*) list->head->data == EXPECTED[i]);
			}
		}
	}
	
	destroy_list(list);
	log_info("PASS\n");
}

void list_get_test()
{
	List* list = setup(NULL);
	
	int EXPECTED[6] = {  rand(), rand(), rand(), rand(), rand(), rand() };
	
	for (size_t i = 0; i < sizeof(EXPECTED) / sizeof(int); i++)
		list_append(list, &EXPECTED[i]);
	
	for (size_t i = 0; i < sizeof(EXPECTED) / sizeof(int); i++)
		assert(*(int*) list_get(list, i) == EXPECTED[i]);
	
	destroy_list(list);
	log_info("PASS\n");
}

void list_remove_test()
{
	List* list = setup(NULL);
	
	int EXPECTED[6] = {  rand(), rand(), rand(), rand(), rand(), rand() };
	
	
	for (size_t i = 0; i < 6; i++)
		list_append(list, &EXPECTED[i]);
	
	for (size_t i = 6; i < 6; i--) {
		printf("%zu\n", i);
		list_remove(list, 1);
		assert(list->size == (6 - (i+1)));
	}
	
	destroy_list(list);
	log_info("PASS\n");
}

void list_tests()
{
	init_list_test();
	list_append_test();
	list_prepend_test();
	list_get_test();
	list_remove_test();
}
