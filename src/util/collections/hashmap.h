#pragma once

#include <stddef.h>

typedef enum {
	MAP_MISSING,
	MAP_FULL,
	MAP_OOM,
	MAP_OK
} MapStatus;

typedef int (*generic)(void*, void*);

typedef struct HashMap HashMap;

HashMap* init_hashmap();
void destroy_hashmap();
MapStatus hashmap_put(HashMap* in, char* key, void* value);
MapStatus hashmap_get(HashMap* in, char* key, void** arg);
MapStatus hashmap_remove(HashMap* in, char* key);
size_t hashmap_length();

