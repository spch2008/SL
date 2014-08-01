#ifndef _SL_CIRCLE_QUEUE_
#define _SL_CIRCLE_QUEUE_

struct _sl_circle_queue_t
{
    int *data;
    int  size;

    int  head;
    int  tail;
};

int init_circle_queue(int size);
int destroy_cicle_queue();

int put_circle_item(int fd);
int get_circle_item(int *fd);

int get_circle_queue_size();
bool is_circle_queue_full();
bool is_circle_queue_empty();



#endif
