#include "sl_circle_queue.h"
#include <stdlib.h>

typedef struct _sl_shared_t sl_shared_t;

static sl_shared_t *queue;

int init_queue(int size)
{
    if (size <= 0)
	return -1;

    queue = (sl_shared_t*)malloc(sizeof(sl_shared_t));
    if (queue == NULL)
	return -1;

    queue->head = 0;
    queue->tail = 0;
    queue->size = size;

    queue->data = (int*)malloc(sizeof(int) * queue->size);
    if (queue->data == NULL)
	return -1;

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->empty, NULL);
    pthread_cond_init(&queue->full, NULL);

    return 0;
}


int put_event(int fd)
{
    if (queue == NULL)
	return -1;

    pthread_mutex_lock(&queue->mutex);
    while ((queue->tail + 1) % queue->size == queue->head)
	pthread_cond_wait(&queue->empty, &queue->mutex);

    queue->data[queue->tail] = fd;
    queue->tail = (queue->tail + 1) % queue->size;

    pthread_mutex_unlock(&queue->mutex);

    pthread_cond_signal(&queue->full);

    return 0;
}

int get_event(int *fd)
{
    if (fd == NULL || queue == NULL)
	return -1;

    pthread_mutex_lock(&queue->mutex);
    while (queue->head == queue->tail)
	pthread_cond_wait(&queue->full, &queue->mutex);

    *fd = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->size;

    pthread_mutex_unlock(&queue->mutex);

    pthread_cond_signal(&queue->empty);

    return 0;
}



