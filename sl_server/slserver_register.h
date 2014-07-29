#ifndef _SLSERVER_REGISTER_H_
#define _SLSERVER_REGISTER_H_

#include "slserver_core.h"
#include "sl_circle_queue.h"

#include <stdio.h>

typedef void *(*thread_func)(void *);

struct _sl_server_type_pool_t
{
    thread_func master_handler;
    thread_func worker_handler;
};


typedef struct _sl_server_type_pool_t sl_server_type_pool_t;

void *a(void *p)
{
    printf("Hello");
}

void *b(void *p)
{
    printf("World!\n");
}

typedef int (*init_func)(int);
typedef int (*put_func)(int);
typedef int (*get_func)(int*);

struct _sl_server_queue_t
{
    init_func init;
    get_func  get;
    put_func  put;
};

typedef struct _sl_server_queue_t sl_server_queue_t;

sl_server_queue_t sl_queue[] = 
{
    {init_circle_queue, get_circle_event, put_circle_event} 
};

sl_server_type_pool_t g_server_type_pool [] = 
{
    {a, b}
};


#endif
