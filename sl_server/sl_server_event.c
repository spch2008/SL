#include "sl_server_event.h"

static pthread_key_t g_key;
static pthread_once_t g_once = PTHREAD_ONCE_INIT;

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

void *get_read_buf()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return NULL;
    return tdata->read_buf;
}

int   get_read_size()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return NULL;
    return tdata->read_size;
}

void *get_write_buf()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return NULL;
    return tdata->write_buf;
}

int   get_write_size()
{
    sl_server_tdata_t *tdata = get_tdata();
    if (tdata == NULL)
	return NULL;
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
