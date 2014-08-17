#ifndef _SL_NAMING_SERVER_H_
#define _SL_NAMING_SERVER_H_
#include <pthread.h>

#define MAX_PATH 255
#define MAX_THREAD 10

typedef struct _sl_naming_server_t
{
    char server_name[MAX_PATH];
   
    char listen_ip[20]; 
    char listen_port[10];
    int  listen_fd;

    int thread_tid[MAX_THREAD];
    int real_thread_num;

    pthread_t tid[3];

}sl_naming_server_t;


sl_naming_server_t *init_naming_server();
void destroy_naming_server(sl_naming_server_t *server);
int  run_naming_server(sl_naming_server_t *server);
int  join_naming_server(sl_naming_server_t *server);

#endif
