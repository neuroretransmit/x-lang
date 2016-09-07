#include "list.h"

#include <stdlib.h>

#include "../mem_utils.h"
#include "../debug.h"

List* init_list()
{
	return calloc(1, sizeof(List));
}

List* init_list_objects(void (*destructor)(void* obj))
{
	List* list = calloc(1, sizeof(List));
	list->destructor = (destructor) ? destructor : destroy;

	return list;
}

ListNode* init_list_node(void* data)
{
	ListNode* node = calloc(1, sizeof(ListNode));
	node->data = data;

	return node;
}

void destroy_list(List* list)
{
	if (!list)
		log_kill("list was null\n");

	while (list->size)
		list_remove(list, list->size - 1);

	destroy(list);
}

void destroy_list_node(List* list, ListNode* node)
{
	if (!list)
		log_kill("list was null\n");
	else if (!list->head || !list->tail)
		log_kill("list was empty\n");
	else if (!node)
		log_kill("node was null\n");

	// Join surrounding nodes
	if (list->head == node) {			// Head - reassign
		list->head = node->next;
		list->head->prev = node->next;
	} else if (list->tail == node) { 	// Tail - reassign
		list->tail = node->prev;
		list->tail->next = node->prev;
	} else {							// Between
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	if (list->destructor != NULL && node->data != NULL)
		list->destructor(node->data);
	else if (node->data != NULL)
		destroy(node->data);

	destroy(node);
}

static ListNode* get_node(List* list, size_t pos)
{
	ListNode* tmp = list->head;

	for (size_t i = 0; i < pos; i++)
		tmp = tmp->next;

	return tmp;
}

void list_remove(List* list, size_t pos)
{
	if (!list)
		log_kill("list was null\n");
	else if ((pos + 1) > list->size)
		log_kill("out of bounds\n");

	destroy_list_node(list, get_node(list, pos));
	list->size -= 1;
}

void list_append(List* list, void* data)
{
	if (!list)
		log_kill("list was null\n");

	ListNode* node = init_list_node(data);

	node->prev = (list->tail) ? list->tail : node;
	node->prev->next = node;
	node->next = node;
	list->tail = node;

	list->size += 1;

	if (list->size == 1)
		list->head = node;
}

void list_prepend(List* list, void* data)
{
	if (!list)
		log_kill("list was null\n");

	ListNode* node = init_list_node(data);

	node->prev = node;
	node->next = (list->head) ? list->head : node;
	node->next->prev = node;
	list->head = node;

	list->size += 1;

	if (list->size == 1)
		list->tail = node;
}

void* list_get(List* list, size_t pos)
{
	return get_node(list, pos)->data;
}

