#include "sl_circle_queue.h"
#include <stdlib.h>

typedef struct _sl_circle_queue_t sl_circle_queue_t;

static sl_circle_queue_t *queue;

int init_circle_queue(int size)
{
    if (size <= 0)
	return -1;

    queue = (sl_circle_queue_t*)malloc(sizeof(sl_circle_queue_t));
    if (queue == NULL)
	return -1;

    queue->head = 0;
    queue->tail = 0;
    queue->size = size;

    queue->data = (int*)malloc(sizeof(int) * queue->size);
    if (queue->data == NULL)
	return -1;

    return 0;
}

int destroy_circle_queue()
{
    if (queue->data != NULL)
	free(queue->data);

    return 0;
}

int put_circle_item(int fd)
{
    if (queue == NULL)
	return -1;

    if (is_circle_queue_full())
	return -1;

    queue->data[queue->tail] = fd;
    queue->tail = (queue->tail + 1) % queue->size;

    return 0;
}

int get_circle_item(int *fd)
{
    if (fd == NULL || queue == NULL)
	return -1;

    if (is_circle_queue_empty())
	return -1;

    *fd = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->size;

    return 0;
}

int get_circle_queue_size()
{
    if (queue == NULL)
	return -1;
    else
	return queue->size;
}

int is_circle_queue_full()
{
    if (queue == NULL)
	return 1;
    
    if ((queue->tail + 1) % queue->size == queue->head)
	return 1;
    return 0;
}

int is_circle_queue_empty()
{
    if (queue == NULL)
	return 0;

    if (queue->tail == queue->head)
	return 1;

    return 0;
}

