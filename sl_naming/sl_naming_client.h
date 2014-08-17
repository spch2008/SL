#ifndef _SL_NAMING_CLIENT_H_
#define _SL_NAMING_CLIENT_H_

struct _sl_naming_client_t
{
    char sl_server_ip[20];
    char sl_server_port[10];

    char name_server_ip[20];
    char name_server_port[10];
};

typedef struct _sl_naming_client_t sl_naming_client_t;


sl_naming_client_t *sl_naming_client_init();
int sl_naming_client_random_get(sl_naming_client_t *client, char *ip, char *port);



#endif
