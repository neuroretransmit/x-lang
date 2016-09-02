#include "fifo.h"

#include <stdlib.h>

FIFO* init_fifo()
{
	return calloc(1, sizeof(FIFO));
}

FIFO* init_fifo_objects(void (*destructor)(void* data))
{
	FIFO* fifo = calloc(1, sizeof(FIFO));
	fifo->destructor = destructor;

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
				free(fifo->head->data);
				fifo->head->data = NULL;

				while ((node = fifo->head->next)) {
					free(node->data);
					node->data = NULL;
				}
			}
		}
	}

	free(fifo->head);
	fifo->head = NULL;
	free(fifo);
	fifo = NULL;
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

	free(node);
	node = NULL;

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

