#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "sl_client_core.h"
#include "../sl_common/sl_utils.h"
#include "../sl_io/sl_io.h"

sl_client_t * sl_client_create(char *name);
int sl_client_connect(sl_client_t *client, char *ip, int port);

void *sl_client_get_read_buf(sl_client_t *client);
void *sl_client_get_write_buf(sl_client_t *client);

int sl_client_get_read_size(sl_client_t *client);
int sl_client_get_write_size(sl_client_t *client);

#endif
