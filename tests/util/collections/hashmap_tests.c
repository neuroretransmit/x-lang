#include "hashmap_tests.h"

#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <util/log.h>
#include <util/collections/list.h>
#include <util/mem_utils.h>
#include <util/collections/hashmap.h>

static void init_hashmap_test()
{
	HashMap* map = init_hashmap(NULL, NULL);
	assert(map->table);
	log_info("PASS\n");
	destroy_hashmap(map);
}

static void hashmap_put_test()
{
	HashMap* map = init_hashmap(NULL, NULL);
	HashNode* node[1024];

	List* node_ptrs = init_list_objects(NULL);

	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_append(node_ptrs, node[i] = malloc(sizeof(HashNode)));
		hashmap_put(map, node[i], key);
		assert(hashmap_get(map, key));
	}

	log_info("PASS\n");
	
	for (unsigned i = 0; i < 1024; i++)
		destroy(node[i]);
	
	destroy_list(node_ptrs);
	destroy_hashmap(map);
}

static void hashmap_remove_test()
{
	HashMap* map = init_hashmap(NULL, NULL);
	HashNode* node[1024];
	List* node_ptrs = init_list_objects(NULL);

	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_append(node_ptrs, node[i] = malloc(sizeof(HashNode)));
		hashmap_put(map, node[i], key);
	}

	for (unsigned i = 1023; i != 0; --i) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		hashmap_remove(map, key);
		assert(map->count == i);
		assert(!hashmap_get(map, key));
	}

	log_info("PASS\n");

	for (unsigned i = 0; i < 1024; i++)
		destroy(node[i]);
	
	destroy_list(node_ptrs);
	destroy_hashmap(map);
}

void hashmap_tests()
{
	init_hashmap_test();
	hashmap_put_test();
	hashmap_remove_test();
}
