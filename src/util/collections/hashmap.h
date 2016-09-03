#pragma once

#include <string.h>
#include <stdbool.h>

typedef struct HashNode {
	size_t hash;
	struct HashNode* next;
} HashNode;

typedef size_t (*hash_func)(void* key);
typedef bool (*cmp_func)(HashNode* node, void* key);

typedef struct HashMap {
	struct HashNode** table;
	size_t len, count;
	hash_func hash;
	cmp_func cmp;
} HashMap;

HashMap* init_hashmap(hash_func hash, cmp_func cmp);
void destroy_hashmap(HashMap* map);
HashNode* hashmap_get(HashMap* map, void* key);
int hashmap_put(HashMap* map, HashNode* node, void* key);
HashNode* hashmap_remove(HashMap* map, void* key);
