#ifndef _SL_LOG_H_
#define _SL_LOG_H_

#include <stdio.h>

#define SL_LOG_ERROR(fmt, arg...) printf(fmt, arg)
#define SL_LOG_WARNING(fmt, arg...) printf(fmt, arg)
#define SL_LOG_TRACE(fmt, arg...) printf(fmt, arg)
#define SL_LOG_DEBUG(fmt, arg...) printf(fmt, arg)


#endif
