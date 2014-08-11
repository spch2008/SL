#include "sl_server_event.h"
#include "sl_server_register.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "../sl_log/sl_log.h"
#include "../sl_io/sl_io.h"

extern sl_server_event_t g_server_event_pool[];

static pthread_key_t g_key;
static pthread_once_t g_once = PTHREAD_ONCE_INIT;

int default_call_back()
{
    int ret;
    sl_server_tdata_t  *tdata;
    sl_server_t *server;

    tdata = get_tdata();
    server = get_server();
    

    //res = read(tdata->client_fd, tdata->read_buf, tdata->read_size);   
    //((char*)(tdata->read_buf))[tdata->read_size-1] = '\0';

    sl_io_head_t *req = (sl_io_head_t*)sl_server_get_read_buf();
    sl_io_head_t *res = (sl_io_head_t*)sl_server_get_write_buf();
    int req_size = sl_server_get_read_size();

    sl_io_read(tdata->client_fd, req, req_size);

    
    server->handler();
    
    //((char*)(tdata->write_buf))[tdata->write_size-1] = '\0';
    //write(tdata->client_fd, tdata->write_buf, strlen(tdata->write_buf));

    sl_io_write(tdata->client_fd, res);
    
    return 0;
}


static void create_key_once(void)
{
    pthread_key_create(&g_key, NULL);
}

int set_tdata(sl_server_tdata_t *data)
{
    void *ptr = NULL;
    pthread_once(&g_once, create_key_once);
    if ((ptr = pthread_getspecific(g_key)) == NULL) {
        ptr = data;
        pthread_setspecific(g_key, ptr);
    }
    return 0;
}


sl_server_tdata_t *get_tdata()
{
    void *ptr = pthread_getspecific(g_key);
    return (sl_server_tdata_t*)ptr;
}

void *sl_server_get_read_buf()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return NULL;
    return tdata->read_buf;
}

int sl_server_get_read_size()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return 0;
    return tdata->read_size;
}

void *sl_server_get_write_buf()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return NULL;
    return tdata->write_buf;
}

int sl_server_get_write_size()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return 0;
    return tdata->write_size;
}

sl_server_t *get_server()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return NULL;
    return tdata->parent;
}

void default_callback()
{
}
