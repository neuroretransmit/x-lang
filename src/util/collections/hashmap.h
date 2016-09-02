#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum {
	MAP_MISSING,
	MAP_FULL,
	MAP_OOM,
	MAP_OK
} MapStatus;

typedef int (*generic)(void*, void*);

struct HashMap;

typedef struct {
	char* key;
	bool in_use;
	void* data;
} HashMapElement;

typedef struct {
	size_t table_size;
	size_t size;
	HashMapElement* data;
} HashMap_Map;

typedef HashMap_Map HashMap;

HashMap* init_hashmap();
void destroy_hashmap();
MapStatus hashmap_put(HashMap* in, char* key, void* value);
MapStatus hashmap_get(HashMap* in, char* key, void** arg);
MapStatus hashmap_remove(HashMap* in, char* key);
size_t hashmap_length();

