#include "sl_server_queue.h"
#include <pthread.h>

struct shared_t
{
    pthread_mutex_t mutex;
    pthread_cond_t  full;
    pthread_cond_t  empty;
};

static struct shared_t g_shared;

int sl_server_queue_init(sl_server_t *server, int size)
{
    if (server == NULL)
	return -1;

    pthread_mutex_init(&g_shared.mutex, NULL);
    pthread_cond_init(&g_shared.full, NULL);
    pthread_cond_init(&g_shared.empty, NULL);

    return g_server_queue[server->queue_type].init(size);
}


int sl_server_queue_destroy(sl_server_t *server)
{
    if (server == NULL)
	return -1;

    return g_server_queue[server->queue_type].destroy();
}


int sl_server_queue_put_item(sl_server_t *server, int item)
{
    if (server == NULL)
	return -1;

    pthread_mutex_lock(&g_shared.mutex);

    while (g_server_queue[server->queue_type].full())
	pthread_cond_wait(&g_shared.empty, &g_shared.mutex);

    g_server_queue[server->queue_type].put(item);
    
    pthread_mutex_unlock(&g_shared.mutex);
    pthread_cond_signal(&g_shared.full);

    return 0;
}

int sl_server_queue_get_item(sl_server_t *server, int *item)
{
    if (server == NULL)
	return -1;

    pthread_mutex_lock(&g_shared.mutex);

    while (g_server_queue[server->queue_type].empty())
	pthread_cond_wait(&g_shared.full, &g_shared.mutex);
 
    g_server_queue[server->queue_type].get(item);
   
    pthread_mutex_unlock(&g_shared.mutex);
    pthread_cond_signal(&g_shared.empty);

    return 0;
}


