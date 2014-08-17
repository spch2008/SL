#include "sl_naming_client.h"
#include "sl_naming_common.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

sl_naming_client_t *sl_naming_client_init()
{
    sl_naming_client_t *client = (sl_naming_client_t*)malloc(sizeof(sl_naming_client_t));
    if (client == NULL)
	return NULL;

    strncpy(client->name_server_ip, "127.0.0.1", sizeof(client->name_server_ip));
    strncpy(client->name_server_port, "8888", sizeof(client->name_server_port));


    return client;
}



int sl_naming_client_random_get(sl_naming_client_t *client, char *ip, char *port)
{
    if (client == NULL)
	return -1;

    int fd, res;
    sl_naming_head_t head;
    struct addrinfo hint, *result;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    res = getaddrinfo(client->name_server_ip, client->name_server_port, &hint, &result);
    if (res == -1)
	return -1;

    fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd == -1)
	goto ERR;

    res = connect(fd, result->ai_addr, result->ai_addrlen);
    if (res == -1)
	goto ERR;

    
    head.io_head.reserved = 1;
    head.io_head.body_len = 5;
    
    strcpy(head.data, "Hello");

    sl_io_write(fd, &head);

    sl_io_read(fd, &head, sizeof(sl_naming_head_t));

    sl_naming_head_t *name_head = (sl_naming_head_t*)&head;

    char *ips = name_head->ip;
    printf("ip : %s\n", ips);

    return 0; 

ERR:
    freeaddrinfo(result);
    return -1;   
}

