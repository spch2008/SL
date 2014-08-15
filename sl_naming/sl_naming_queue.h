#ifndef _SL_NAMING_QUEUE_H_
#define _SL_NAMING_QUEUE_H_

typedef struct _queue_entry
{
    int   head;
    int   tail;
    int   size;
    void **data;
}sl_naming_queue_t;

sl_naming_queue_t *queue_init(int queue_size);
void queue_destroy(sl_naming_queue_t *queue);
void *queue_get(sl_naming_queue_t *queue);
int queue_put(sl_naming_queue_t *queue, void *entry);

#endif
