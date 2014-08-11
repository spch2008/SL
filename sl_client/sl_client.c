#include "sl_client.h"
#include "../sl_io/sl_io.h"
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

int _sl_client_set_client_name(sl_client_t *client, const char *name)
{    
    if (client == NULL)
        return -1;
 
    strncpy(client->client_name, name, sizeof(client->client_name));
    client->client_name[sizeof(client->client_name)-1] = '\0';
 
    return 0;
 }


sl_client_t *sl_client_create(char *name)
{
    sl_client_t *client = (sl_client_t*)malloc(sizeof(sl_client_t));
    if (client == NULL)
	return NULL;

    client->port  = -1;
    client->ip[0] = '\0';

    client->read_size  = 1024;
    client->write_size = 1024;

    client->read_buf  = (char*)malloc(client->read_size);
    client->write_buf = (char*)malloc(client->write_size);
    
    client->fd = -1;

    if (name == NULL)
        _sl_client_set_client_name(client, "default client");
    else
        _sl_client_set_client_name(client, name);

    return client;
}

int sl_client_connect(sl_client_t *client, char *ip, int port)
{
    if (ip == NULL || client == NULL)
	return -1;

    struct addrinfo hint, *result;
    int res, fd;
    char s_port[20];

    memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    client->port = port;
    strcpy(client->ip, ip);

    sprintf(s_port, "%d", client->port);
        
    res = getaddrinfo(ip, s_port, &hint, &result);
    if (res == -1)
	return -1;

    fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd == -1)
	return -1;

    res = connect(fd, result->ai_addr, result->ai_addrlen);
    if (res == -1)
	return -1;

    client->fd = fd;

    return 0;
}

int sl_client_talk_server(sl_client_t *client)
{
    if (client == NULL)
	return -1;
    
    sl_io_head_t *req_head = (sl_io_head_t*)sl_client_get_write_buf(client);

    // write buf to server
    //client->write_buf[client->write_size - 1] = '\0';
    //write(client->fd, client->write_buf, strlen(client->write_buf));
    sl_io_write(client->fd, req_head);


    sl_io_head_t *res_head = (sl_io_head_t*)sl_client_get_read_buf(client);
    int res_size = sl_client_get_read_size(client) - sizeof(sl_io_head_t);
    // read buf from server
    //read(client->fd, client->read_buf, client->read_size-1);
    //client->read_buf[client->read_size-1] = '\0';
    sl_io_read(client->fd, res_head, res_size);

    return 0;
}

int sl_client_get_read_size(sl_client_t *client)
{
    if (client == NULL)
	return 0;

    return client->read_size;
}

int sl_client_get_write_size(sl_client_t *client)
{
    if (client == NULL)
	return 0;

    return client->write_size;
}

void *sl_client_get_read_buf(sl_client_t *client)
{
    if (client == NULL)
	return NULL;

    return client->read_buf;
}

void *sl_client_get_write_buf(sl_client_t *client)
{
    if (client == NULL)
	return NULL;

    return client->write_buf;
}
