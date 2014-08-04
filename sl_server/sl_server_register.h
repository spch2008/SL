#ifndef _SLSERVER_REGISTER_H_
#define _SLSERVER_REGISTER_H_

#include "sl_server_core.h"
#include "sl_circle_queue.h"

#include <stdio.h>


typedef int   (*event_init)(int);
typedef int   (*event_listen)(int);
typedef void *(*event_handler)(void *);

struct _sl_server_event_t
{
    event_init    init;
    event_listen  listen;
    event_handler master;
    event_handler worker;
};
typedef struct _sl_server_event_t sl_server_event_t;


typedef int  (*queue_init)(int);
typedef int  (*queue_fini)();
typedef int  (*queue_put)(int);
typedef int  (*queue_get)(int*);
typedef bool (*queue_bool)();

struct _sl_server_queue_t
{
    queue_init init;
    queue_get  get;
    queue_put  put;
    queue_bool full;
    queue_bool empty;
    queue_fini destroy;
};
typedef struct _sl_server_queue_t sl_server_queue_t;

sl_server_queue_t g_server_queue[] = 
{
    { init_circle_queue, 
      get_circle_item,
      put_circle_item, 
      is_circle_queue_full,
      is_circle_queue_empty,
      destroy_circle_queue
    } 
};

sl_server_event_t g_server_event[] = 
{
    {NULL, NULL, NULL, NULL}
};


#endif
