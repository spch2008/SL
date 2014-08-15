#ifndef _SL_CONF_H_
#define _SL_CONF_H_
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATH 255
#define MAX_LINE 100

typedef struct _sl_conf_entry_t
{
    char key[MAX_PATH];
    char val[MAX_PATH];
}sl_conf_entry_t;

struct _sl_conf_data_t
{
    char file_name[MAX_PATH];
    FILE *conf_file;
    
    sl_conf_entry_t conf_data[MAX_LINE];
    int real_conf_num;
};

typedef struct _sl_conf_data_t sl_conf_data_t;


sl_conf_data_t *sl_conf_init(const char *conf_file);
void sl_conf_close(sl_conf_data_t *conf);
int sl_conf_getint(const sl_conf_data_t *conf, const char *conf_name,
                   int *conf_value, const int *default_value);
int sl_conf_getstr(const sl_conf_data_t *conf, const char *conf_name, 
                   char *conf_value, size_t conf_size, const char *default_value);

#endif
