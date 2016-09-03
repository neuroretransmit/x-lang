#include "hashmap_tests.h"

#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../src/util/log.h"
#include "../../../src/util/collections/list.h"
#include "../../../src/util/collections/hashmap.h"

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
	HashNode* node = NULL;
	
	List* node_ptrs = init_list_objects(NULL);
	
	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_append(node_ptrs, node = malloc(sizeof(HashNode)));
		hashmap_put(map, node, key);
		assert(hashmap_get(map, key));
	}
	
	log_info("PASS\n");
	
	destroy_list(node_ptrs);
	destroy_hashmap(map);
}

static void hashmap_remove_test()
{
	HashMap* map = init_hashmap(NULL, NULL);
	
	HashNode* node = NULL;
	
	List* node_ptrs = init_list_objects(NULL);
	
	for (unsigned i = 0; i < 1024; i++) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		list_append(node_ptrs, node = malloc(sizeof(HashNode)));
		hashmap_put(map, node, key);
	}
	
	for (unsigned i = 1023; i!= 0; --i) {
		char key[100];
		sprintf(key, "i_am_the_key%d", i);
		hashmap_remove(map, key);
		assert(map->count == i);
		assert(!hashmap_get(map, key));
	}
	
	log_info("PASS\n");
	
	destroy_list(node_ptrs);
	destroy_hashmap(map);
}

void hashmap_tests()
{
	init_hashmap_test();
	hashmap_put_test();
	hashmap_remove_test();
}