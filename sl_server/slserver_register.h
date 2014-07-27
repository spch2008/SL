#ifndef _SLSERVER_REGISTER_H_
#define _SLSERVER_REGISTER_H_

#include "slserver_core.h"

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

sl_server_type_pool_t g_server_type_pool [] = 
{
    {a, b}
};


#endif
