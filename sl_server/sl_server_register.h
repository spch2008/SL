#ifndef _SLSERVER_REGISTER_H_
#define _SLSERVER_REGISTER_H_

#include "sl_server_core.h"
#include "sl_circle_queue.h"

#include <stdio.h>

typedef void *(*thread_func)(void *);

struct _sl_server_type_pool_t
{
    thread_func master_handler;
    thread_func worker_handler;
};
typedef struct _sl_server_type_pool_t sl_server_type_pool_t;


typedef int (*init_func)(int);
typedef int (*destroy_func)();
typedef int (*put_func)(int);
typedef int (*get_func)(int*);
typedef bool (*bool_func)();

struct _sl_server_queue_t
{
    init_func init;
    get_func  get;
    put_func  put;
    destroy_func size;
    bool_func full;
    bool_func empty;
    destroy_func destroy;
};
typedef struct _sl_server_queue_t sl_server_queue_t;

sl_server_queue_t g_server_queue[] = 
{
    { init_circle_queue, 
      get_circle_item,
      put_circle_item, 
      get_circle_queue_size,
      is_circle_queue_full,
      is_circle_queue_empty,
      destroy_circle_queue
    } 
};

sl_server_type_pool_t g_server_pool [] = 
{
    {0, 0}
};


#endif
