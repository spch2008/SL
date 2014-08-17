#ifndef _SL_NAMING_SERVER_H_
#define _SL_NAMING_SERVER_H_

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

}sl_naming_server_t;




#endif
