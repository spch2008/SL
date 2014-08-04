#ifndef _SL_SERVER_EVENT_H_
#define _SL_SERVER_EVENT_H_

int set_tdata(sl_server_tdata_t *data);
sl_server_tdata_t *get_tdata();

void *get_read_buf();
int   get_read_size();

void *get_write_buf();
int   get_write_size();

sl_server_t *get_server();











#endif
