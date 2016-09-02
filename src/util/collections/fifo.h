#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct FIFONode {
	void* data;
	struct FIFONode* next;
} FIFONode;

typedef struct {
	FIFONode* head;
	FIFONode* tail;
	size_t size;
	void (*destructor)(void* obj);
} FIFO;

FIFO* init_fifo();
FIFO* init_fifo_objects(void (*destructor)(void* data));

void destroy_fifo(FIFO* fifo);
void fifo_push(FIFO* fifo, void* data);
void* fifo_pop(FIFO* fifo);
void* fifo_peek(FIFO* fifo);

bool fifo_empty(FIFO* fifo);

