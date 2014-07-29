#ifndef _SLSERVER_CORE_H_
#define _SLSERVER_CORE_H_

#include <pthread.h>
#include <sys/socket.h>


typedef int (*callback_func)();
typedef unsigned int u_int;

enum serv_type
{
    SELECT, POLL, EPOLL
};


struct _sl_server_t;
struct _sl_server_tdata_t;

typedef struct _sl_server_t sl_server_t;
typedef struct _sl_server_tdata_t sl_server_tdata_t;


struct _sl_server_tdata_t
{
    pthread_t tid;
    int slot;         // sl_server_t array index

    void *read_buf;
    void *write_buf;
    int   read_size;
    int   write_size;

    int fd;
    struct sockaddr client_addr;

    sl_server_t *parent;
};



struct _sl_server_t
{
    // server config
    char serv_name[255];
    u_int serv_type;

    // thread config
    int thread_num;
    int real_thread_num;

    sl_server_tdata_t *thread_data;
    int read_size;
    int write_size;

    // socket info
    u_int listen_port;
    u_int backlog;
    int   serv_socket;

    // call_back
    callback_func handler; 
};


#endif
