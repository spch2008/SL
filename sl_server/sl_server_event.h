#ifndef _SL_SERVER_EVENT_H_
#define _SL_SERVER_EVENT_H_
#include "sl_server_core.h"

int set_tdata(sl_server_tdata_t *data);
sl_server_tdata_t *get_tdata();

void *sl_server_get_read_buf();
int   sl_server_get_read_size();

void *sl_server_get_write_buf();
int   sl_server_get_write_size();

sl_server_t *get_server();

#endif
