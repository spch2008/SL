#include "sl_naming_queue.h"
#include <stdlib.h>



sl_naming_queue_t *queue_init(int size)
{
    if (size <= 0)
	return NULL;

    sl_naming_queue_t *queue = (sl_naming_queue_t*)malloc(sizeof(sl_naming_queue_t));    
    if (queue == NULL)
	return NULL;

    queue->size = size;
    queue->data = malloc(size * sizeof(void*));
    if (queue->data == NULL)
	return NULL;

    return queue;
}


void queue_destroy(sl_naming_queue_t *queue)
{
    if (queue == NULL)
	return;

    int i;
    for (i = 0; i < queue->size; i++)
    {
        if (queue->data[i] != NULL)
	    free(queue->data[i]);
    }

    free(queue->data);
    free(queue);
}

void *queue_get(sl_naming_queue_t *queue)
{
    if (queue == NULL)
	return NULL;

    if (queue->head == queue->tail)
	return NULL;

    int slot = queue->head;
    queue->head = (queue->head + 1) % queue->size;

    return queue->data[slot];
}

int queue_put(sl_naming_queue_t *queue, void *entry)
{
    if (queue == NULL || entry == NULL)
	return -1;

    if (queue->head == (queue->tail + 1) % queue->size)
	return -1;

    int slot = queue->tail;
    queue->tail =  (queue->tail + 1) % queue->size;

    queue->data[slot] = entry;
    return 0;
}
