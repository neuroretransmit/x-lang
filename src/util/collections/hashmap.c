#include "hashmap.h"

#include <stdlib.h>
#include <stdbool.h>

#include "hashmap.h"
#include "../mem_utils.h"
#include "../log.h"

#define MIN_SLOTS	16
#define MULTIPLIER	37

size_t hash32(char* str)
{
	unsigned int h = 0;
	unsigned char* p;

	for (p = (unsigned char*)str; *p != '\0'; p++)
		h = MULTIPLIER * h + *p;

	return h; // or, h % ARRAY_SIZE;
}

static bool hash_cmp(HashNode* node, char* key)
{
	if (node) {
		if (node->hash == hash32(key))
			return true;
	} else {
		log_warn("Node not set.");
	}

	return false;
}

HashMap* init_hashmap(hash_func hash, cmp_func cmp)
{
	HashMap* map = malloc(sizeof(HashMap));
	map->len = MIN_SLOTS;
	map->table = calloc(map->len, sizeof(HashNode*));
	map->count = 0;
	map->hash = (hash) ? hash : (hash_func) &hash32;
	map->cmp = (cmp) ? cmp : (cmp_func) &hash_cmp;

	return map;
}

void destroy_hashmap(HashMap* map)
{
	destroy(map->table);
	destroy(map);
}

static int hashmap_grow(HashMap* map)
{
	/* first, allocate more room for the table */
	HashNode** newtable = realloc(map->table, map->len * 2 * sizeof(HashNode*));

	if (newtable == NULL)
		return -1;

	map->table = newtable;

	/* then, split all nodes from the lower half of the table
	   to either lower or upper half of the table */
	for (size_t i = 0; i < map->len; ++i) {
		HashNode* node = map->table[i], *next;
		HashNode* a = NULL, *b = NULL;

		while (node) {
			next = node->next;

			if (node->hash & map->len) {
				/* upper half */
				node->next = b;
				b = node;
			} else {
				/* lower half */
				node->next = a;
				a = node;
			}

			node = next;
		}

		map->table[i] = a;
		map->table[i + map->len] = b;
	}

	map->len *= 2;
	return 0;
}

static int hashmap_shrink(HashMap* map)
{
	size_t i;

	/* first, fold the upper half of the table to top of the lower half */
	map->len /= 2;

	for (i = 0; i < map->len; ++i) {
		HashNode* prev = map->table[i];
		HashNode* next = map->table[i + map->len];

		if (prev == NULL)
			map->table[i] = next;
		else {
			while (prev->next)
				prev = prev->next;

			prev->next = next;
		}
	}

	/* then, release unneeded memory */
	HashNode** newtable = realloc(map->table, map->len * sizeof(HashNode*));

	if (newtable == NULL)
		return -1;

	map->table = newtable;
	return 0;
}

HashNode* hashmap_get(HashMap* map, void* key)
{
	HashNode* node = map->table[map->hash(key) & (map->len - 1)];

	while (node) {
		if (map->cmp(node, key))
			return node;

		node = node->next;
	}

	return NULL;
}

int hashmap_put(HashMap* map, HashNode* node, void* key)
{
	size_t slot;
	node->hash = map->hash(key);
	slot = node->hash & (map->len - 1);
	node->next = map->table[slot];
	map->table[slot] = node;
	map->count++;

	if (map->count > map->len * 3)
		hashmap_grow(map);

	return 0;
}

HashNode* hashmap_remove(HashMap* map, void* key)
{
	size_t slot = map->hash(key) & (map->len - 1);
	HashNode* node = map->table[slot], *prev = NULL;

	while (node) {
		if (map->cmp(node, key)) {
			if (prev != NULL)
				prev->next = node->next;
			else
				map->table[slot] = node->next;

			map->count--;

			if (map->count < map->len / 4 && map->len > MIN_SLOTS)
				hashmap_shrink(map);

			return node;
		}

		prev = node;
		node = node->next;
	}

	return NULL;
}
