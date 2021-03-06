#ifndef _SL_SERVER_H_
#define _SL_SERVER_H_

#include "sl_server_core.h"
#include "../sl_log/sl_log.h"
#include "../sl_common/sl_utils.h"
#include "sl_server_event.h"

sl_server_t *sl_server_create(const char *servname);
int sl_server_set_callback(sl_server_t *server, callback_func func);
int sl_server_run(sl_server_t *server);

static int sl_server_listen(sl_server_t *server);
static int sl_server_set_server_name(sl_server_t *server, const char *name);
static int sl_server_bind(sl_server_t *server);
static int sl_server_init_thread(sl_server_t *server);



#endif
