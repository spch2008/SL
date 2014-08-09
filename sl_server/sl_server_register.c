#include "sl_server_register.h"

sl_server_queue_t g_server_queue_pool[] = 
{
    { init_circle_queue, 
      get_circle_item,
      put_circle_item, 
      is_circle_queue_full,
      is_circle_queue_empty,
      destroy_circle_queue
    },
    { NULL, NULL, NULL, NULL, NULL, NULL} 
};

sl_server_event_t g_server_event_pool[] = 
{
    {epoll_init, epoll_listen, epoll_master, epoll_worker},
    {NULL, NULL, NULL, NULL}
};


