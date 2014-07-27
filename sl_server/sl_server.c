#include "sl_server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "../sl_log/sl_log.h"

sl_server_t *sl_server_create(const char *servname)
{
	sl_server_t *server = (sl_server_t*)malloc(sizeof(sl_server_t));
    if (server == NULL)
		return NULL;

    server->thread_num = 0;
    server->real_thread_num = 0;

    server->listen_port = 0;
    server->backlog = 2048;
    server->serv_socket = -1;

    server->handler = NULL;
    
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


