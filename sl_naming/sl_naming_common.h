#ifndef _SL_NAMING_COMMON_H_
#define _SL_NAMING_COMMON_H_

#include "../sl_io/sl_io.h"

struct _sl_naming_head_t
{
    sl_io_head_t io_head;
    char ip[20];
    char port[10];
    char data[50];
};

typedef struct _sl_naming_head_t sl_naming_head_t;

#endif
