#include "fifo.h"

#include <stdlib.h>

#include "../mem_utils.h"

FIFO* init_fifo()
{
	return calloc(1, sizeof(FIFO));
}

FIFO* init_fifo_objects(void (*destructor)(void* data))
{
	FIFO* fifo = calloc(1, sizeof(FIFO));
	fifo->destructor = (destructor) ? destructor : &destroy;

	return fifo;
}

void destroy_fifo(FIFO* fifo)
{
	FIFONode* node = NULL;

	if (fifo->head) {
		if (fifo->head->data) {
			if (fifo->destructor) {
				fifo->destructor(fifo->head->data);

				while ((node = fifo->head->next)) {
					fifo->destructor(node->data);
				}
			} else {
				destroy(fifo->head->data);

				while ((node = fifo->head->next))
					destroy(node->data);
			}
		}
	}

	destroy(fifo->head);
	destroy(fifo);
}

void fifo_push(FIFO* fifo, void* data)
{
	FIFONode* node = calloc(1, sizeof(FIFONode));
	node->data = data;
	node->next = NULL;

	if (fifo->tail == NULL) {
		fifo->head = fifo->tail = node;
	} else {
		fifo->tail->next = node;
		fifo->tail = node;
	}

	fifo->size++;
}

void* fifo_pop(FIFO* fifo)
{
	FIFONode* node;
	void* data;

	if ((node = fifo->head) == NULL)
		return NULL;

	data = node->data;

	if ((fifo->head = node->next) == NULL)
		fifo->tail = NULL;

	destroy(node);
	fifo->size--;

	return data;
}

void* fifo_peek(FIFO* fifo)
{
	FIFONode* node;

	if ((node = fifo->head) == NULL)
		return NULL;

	return node->data;
}

bool fifo_empty(FIFO* fifo)
{
	return (fifo->head == NULL);
}

