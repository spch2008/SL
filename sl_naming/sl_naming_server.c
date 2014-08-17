#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sl_naming_server.h"
#include "sl_naming_queue.h"
#include "../sl_io/sl_io.h"

#define MAX_SERVER 100
#define MAX_CLIENT 100
#define MAX_CONN 1000

typedef struct _entry_t
{
    int  ip;
    int  fd;
}entry_t;


static sl_naming_queue_t  *g_server_queue;
static sl_naming_queue_t  *g_client_queue;


sl_naming_server_t* init_naming_server()
{
    g_server_queue = queue_init(100);
    g_client_queue = queue_init(100);

    if (g_server_queue == NULL || g_client_queue == NULL)
	return NULL;

    sl_naming_server_t *server = (sl_naming_server_t*)malloc(sizeof(sl_naming_server_t));
    if (server == NULL)
	return NULL;
    
    strncpy(server->listen_ip, "127.0.0.1", sizeof(server->listen_ip));
    strncpy(server->listen_port, "8888", sizeof(server->listen_port));
    server->listen_fd   = -1;
    server->real_thread_num = 0;

    
    return server;
}

int _socket_create_bind(sl_naming_server_t *server)
{
    struct addrinfo hint, *result;
    int res, fd;
  
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags    = AI_PASSIVE;

    res = getaddrinfo(server->listen_ip, server->listen_port, &hint, &result);
    if (res == -1)
	return -1;
    
   fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
   if (fd == -1)
	return -1;

   res = bind(fd, result->ai_addr, result->ai_addrlen);
   if (res == -1)
	return -1;

   res = listen(fd, MAX_CONN); 
   if (res == -1)
	return -1;

   server->listen_fd = fd;
   return 0;
}
 

void destroy_naming_server(sl_naming_server_t *server)
{
    free(server);
}

int handle(int fd, unsigned long ip)
{
    if (fd < 0)
        return -1;

    printf("ip : %d\n", (int)ip);
    
    int res, type;
    sl_io_head_t head;

    res = sl_io_read(fd, &head, sizeof(sl_io_head_t));
    if (res == -1)
	return -1;

    type = head.reserved;
    if (type == 0)
    {
        queue_put_item(g_server_queue, fd);
    }
    else if (type == 1)
    {
        queue_put_item(g_client_queue, fd);
    }
    else
    {
        return -1;
    }

    return 0; 
}

void *master_handler(void *param)
{
   sl_naming_server_t *server = (sl_naming_server_t*)param;
   if (server == NULL)
	return NULL;

   struct sockaddr remote;
   int client_sockets[MAX_CONN/4], addrlen;
   fd_set readfds;
   int max_fd, fd, res;
 
   memset(client_sockets, 0, sizeof(client_sockets));
   addrlen = sizeof(struct sockaddr);

   while (1)
   {
        FD_ZERO(&readfds);
        FD_SET(server->listen_fd, &readfds);
        max_fd = server->listen_fd;

        int i;
        for (i = 0; i < MAX_CONN/4; i++)
        {
            fd = client_sockets[i];
            if (fd > 0)
		FD_SET(fd, &readfds);

            if (fd > max_fd)
		max_fd = fd;
        }

        res = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (res < 0)
	    return NULL;

        if (FD_ISSET(server->listen_fd, &readfds))
	{
	    fd = accept(server->listen_fd, &remote, &addrlen);
            if (fd < 0)
		continue;

            for (i = 0; i < MAX_CONN/4; i++)
            {
                if (client_sockets[i] == 0)
	        {
                    client_sockets[i] = fd;
                    break; 
                }
            }
        }
        else
        {
            for (i = 0; i < MAX_CONN/4; i++)
            {
                fd = client_sockets[i];
                if (FD_ISSET(fd, &readfds))
                {
                   unsigned long ip = ((struct sockaddr_in*)&remote)->sin_addr.s_addr;
                   res =  handle(fd, ip);
                   if (res != 0)
		       client_sockets[i] = 0;
                }
            }
        }
       
   }
   return NULL;
}



void *client_handler(void *param)
{
    
}


void *server_handler(void *param)
{
}


