#ifndef _SL_SERVER_QUEUE_H_
#define _SL_SERVER_QUEUE_H_

#include "sl_server_core.h"
#include "sl_server_register.h"


int sl_server_queue_init(sl_server_t *server, int size);
int sl_server_queue_put_item(sl_server_t *server, int item);
int sl_server_queue_get_item(sl_server_t *server, int *item);
int sl_server_queue_destroy(sl_server_t *server);


#endif
