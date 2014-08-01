#include "sl_server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "../sl_log/sl_log.h"
#include "sl_server_register.h"
#include <pthread.h>

int default_call_back()
{
}

sl_server_t *sl_server_create(const char *servname)
{
    sl_server_t *server = (sl_server_t*)malloc(sizeof(sl_server_t));
    if (server == NULL)
	return NULL;

    server->thread_num = 10;
    server->real_thread_num = 0;
    server->thread_data  = NULL;

    server->listen_port = 8080;
    server->listen_backlog = 2048;
    server->listen_socket = -1;

    server->handler = NULL;
    server->call_back = default_call_back;
    
    server->pool = NULL;
    memset(server->pool_data, 0, sizeof(server->pool));
    
    server->queue_type = 0;    

    server->serv_type = 0;
    if (servname == NULL) {
	sl_server_set_server_name(server, "default server");
    } else {
	sl_server_set_server_name(server, servname);
    }

    return server;
}

int sl_server_set_server_name(sl_server_t *server, const char *name)
{
    if (server == NULL)
	return -1;

    strncpy(server->serv_name, name, sizeof(server->serv_name));
    server->serv_name[sizeof(server->serv_name)-1] = '\0';

    return 0;
}

int sl_server_set_callback(sl_server_t *server, callback_func func)
{
    if (server == NULL)
	return -1;

    server->handler = func;
    return 0;
}

int sl_server_bind(sl_server_t *server)
{
    int ret;
    struct addrinfo *result, hint;
    char port[20];

    memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags    = AI_PASSIVE;
    
    sprintf(port, "%d", server->listen_port);
        
    ret = getaddrinfo(NULL, port, &hint, &result);
    if (ret == -1)
	return -1;
    
    server->serv_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server->serv_socket == -1)
	return -1;

    ret = bind(server->serv_socket, result->ai_addr, result->ai_addrlen);
    if (ret == -1) {
        close(server->serv_socket);
	return -1;
    }
    
    // release
    freeaddrinfo(result);

    return 0;  
}

int sl_server_listen(sl_server_t *server)
{
    int backlog = server->backlog;
    if (listen(server->serv_socket, server->backlog) < 0) {
    	close(server->serv_socket);
        return -1;
    }

    return 0;
}

int sl_server_run(sl_server_t *server)
{
    if (server == NULL)
	return -1;

    int ret;

    ret = sl_server_bind(server);
    if (ret == -1)
	return -1;

    ret = sl_server_listen(server);
    if (ret == -1)
	return -1;

    ret = sl_server_init_thread(server);
    if (ret == -1)
	return -1;

    return sl_server_start_work(server);

    //return g_server_type_pool[server->serv_type].run(server);
}

int sl_server_start_work(sl_server_t *server)
{
    if (server == NULL)
	return -1;

    // check function exist
    sl_server_type_pool_t *pool = &g_server_type_pool[server->serv_type];
    if (pool->master_handler == NULL || pool->worker_handler == NULL)
	return -1;

    // create thread
    int ret;
    ret = pthread_create(&(server->thread_data[server->thread_num].tid), NULL,
                         pool->master_handler, &(server->thread_data[server->thread_num]));

    if (ret == -1)
	return -1;

    int i;
    for (i = 0; i < server->thread_num; i++) {
	ret = pthread_create(&(server->thread_data[i].tid), NULL, pool->worker_handler,
                            &(server->thread_data[i]));
    }

    return 0;
 }

int sl_server_init_thread(sl_server_t *server)
{
    if (server == NULL || server->thread_num <= 0)
	return -1;

    server->thread_data = (sl_server_tdata_t*)malloc(sizeof(sl_server_tdata_t) * (server->thread_num + 1));
    if (server->thread_data == NULL)
	return -1;

    memset(server->thread_data, 0, sizeof(sl_server_tdata_t) * (server->thread_num + 1));

    int i;
    for (i = 0; i < server->thread_num + 1; i++) {
	server->thread_data[i].slot = i;
        server->thread_data[i].parent = server;
    }
 
    for (i = 0; i < server->thread_num + 1; i++) {
	if (server->read_size > 0) {
	    server->thread_data[i].read_size = server->read_size;
            server->thread_data[i].read_buf  = malloc(server->read_size);
            
            if (server->thread_data[i].read_buf == NULL) {
		return -1;
            }

            memset(server->thread_data[i].read_buf, 0, server->thread_data[i].read_size);
        }

        if (server->write_size > 0) {
	    server->thread_data[i].write_size = server->write_size;
            server->thread_data[i].write_buf  = malloc(server->write_size);

            if (server->thread_data[i].write_buf == NULL) {
		return -1;
            }

  	    memset(server->thread_data[i].write_buf, 0, server->thread_data[i].write_size);
        }
    }

    return 0;
}
