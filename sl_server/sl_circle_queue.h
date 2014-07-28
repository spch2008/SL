#ifndef _SL_CIRCLE_QUEUE_
#define _SL_CIRCLE_QUEUE_

#include <pthread.h>

struct _sl_shared_t
{
    pthread_mutex_t mutex;
    pthread_cond_t  empty;
    pthread_cond_t  full;

    int  head;
    int  tail;
    int *data;
    int  size;
};


int init_queue(int size);

int put_event(int fd);
int get_event(int *fd);



#endif
