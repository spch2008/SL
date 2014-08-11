#include "sl_io.h"
#include <unistd.h>

int _sl_io_read(int sock, sl_io_head_t *head, int head_size, void *buf, int buf_size)
{
    read(sock, head, head_size);

    if (head->body_len > buf_size)
	return -1;
    read(sock, buf, head->body_len);

    return 0;
}

int sl_io_read(int sock, void *head, int size)
{
    if (size < sizeof(sl_io_head_t))
	return -1;

    return _sl_io_read(sock, (sl_io_head_t*)head, sizeof(sl_io_head_t), 
                             ((sl_io_head_t*)head + 1), size - sizeof(sl_io_head_t));
}


int sl_io_write(int sock, void *head)
{
    write(sock, head, sizeof(sl_io_head_t) + ((sl_io_head_t*)head)->body_len);

    return 0;
}

