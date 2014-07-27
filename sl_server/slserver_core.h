#ifndef _SLSERVER_CORE_H_
#define _SLSERVER_CORE_H_

typedef int (*callback_func)();
typedef unsigned int u_int;

struct _sl_server_t
{
    // server config
    char serv_name[255];

    // thread config
    int thread_num;
    int real_thread_num;

    // socket info
    u_int listen_port;
    u_int backlog;
    int   serv_socket;

    // call_back
    callback_func handler; 
};

typedef struct _sl_server_t sl_server_t;


#endif
