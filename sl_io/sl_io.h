#ifndef _SL_IO_H_
#define _SL_IO_H_

struct _sl_io_head_t
{
    int id;
    int version;

    int reserved;
    int body_len;
};
typedef struct _sl_io_head_t sl_io_head_t;

int sl_io_read(int sock, void *head, int size);
int sl_io_write(int sock, void *head);



#endif
