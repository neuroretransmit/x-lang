#pragma once

#include <stddef.h>

typedef struct ListNode {
	void* data;
	struct ListNode* prev;
	struct ListNode* next;
} ListNode;

typedef struct {
	ListNode* head;
	ListNode* tail;
	size_t size;
	void (*destructor)(void* data);
} List;

List* init_list();
List* init_list_objects(void (*destructor)(void* data));
ListNode* init_list_node(void* data);
void destroy_list(List* list);
void destroy_list_node(List* list, ListNode* node);

void list_append(List* list, void* data);
void list_prepend(List* list, void* data);
void* list_get(List* list, size_t pos);
void list_remove(List* list, size_t pos);

