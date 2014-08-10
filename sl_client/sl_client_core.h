#ifndef _SL_CLIENT_CORE_H_
#define _SL_CLIENT_CORE_H_

struct _sl_client_t
{
    char client_name[255];

    int  port;
    char ip[20];

    int read_size;
    int write_size;

    char *read_buf;
    char *write_buf;

    int fd;
};

typedef struct _sl_client_t sl_client_t;

#endif
