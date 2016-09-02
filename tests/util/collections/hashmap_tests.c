#include "hashmap_tests.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../src/util/log.h"
#include "../../../src/util/collections/hashmap.h"

HashMap* init_hashmap();
void destroy_hashmap();
MapStatus hashmap_put(HashMap* in, char* key, void* value);
MapStatus hashmap_get(HashMap* in, char* key, void** arg);
MapStatus hashmap_remove(HashMap* in, char* key);
size_t hashmap_length();

/*static const int MAX_KEY_LENGTH = 256;
static const int KEY_COUNT = 1024 * 1024;
static const char* KEY_PREFIX = "foo";

typedef struct {
	char key_str[MAX_KEY_LENGTH];
	int number;
} TestStructure;
*/

static void init_hashmap_test()
{
	HashMap* map = init_hashmap();
	
	assert(map);
	assert(map->data);
	assert(map->size == 0);
	log_info("init_hashmap()...PASSED\n");
	destroy_hashmap(map);
}

/*
bool destroy_hashmap_test()
{
	
}

bool hashmap_get_test()
{
	
}

bool hashmap_put_test()
{
	HashMap* map = init_hashmap();
	int error;
	char key_str[MAX_KEY_LENGTH];
	TestStructure* val;
	
	for (int i = 0; i < KEY_COUNT; i++) {
		val = malloc(sizeof(TestStructure));
		snprintf(val->key_str, MAX_KEY_LENGTH, "%s%d", KEY_PREFIX, i);
		val->number = i;
		
		error = hashmap_put(map, val->key_str, val);
		assert(error == MAP_OK);
	}	
}

bool hashmap_remove_test()
{
	
}

bool hashmap_length_test()
{
	
}
*/
void hashmap_tests()
{
	
	init_hashmap_test();
}