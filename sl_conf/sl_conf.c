#include "sl_conf.h"
#include <stdlib.h>

static int _load_conf(sl_conf_data_t *);
static void _trim(char *);

sl_conf_data_t *sl_conf_init(const char *file_name)
{
    if (file_name == NULL)
    {
        perror("conf file name null\n");
	return NULL;
    }

    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        perror("open conf file error\n");
        return NULL;
    }

    sl_conf_data_t *conf = (sl_conf_data_t*)malloc(sizeof(sl_conf_data_t));
    if (conf == NULL)
    {
        perror("malloc sl_conf_data error");
        return NULL;
    }
    
    conf->real_conf_num = 0;    
    strncpy(conf->file_name, file_name, MAX_PATH);
    conf->conf_file = file;
    
   if ( _load_conf(conf) != 0)
   {
        sl_conf_close(conf);
        return NULL;
   }

   return conf;
}

void sl_conf_close(sl_conf_data_t *conf)
{
    if (conf == NULL)
	return;

    close(conf->conf_file);
    free(conf);
}

int _load_conf(sl_conf_data_t *conf)
{
    if (conf == NULL)
	return -1;

    int slot = 0;
    char buff[MAX_PATH];
    char *p_key, *p_val;

    while (fgets(buff, MAX_PATH, conf->conf_file))
    {
        if (slot >= MAX_LINE)
	    return -1;

        _trim(buff);
        if (buff[0] == '\0' || buff[0] == '#')
            continue;

        buff[MAX_PATH-1] = '\0';

        p_key = strtok(buff, ":");
        _trim(p_key);

        p_val = strtok(NULL, ":");
        _trim(p_val);

        strncpy(conf->conf_data[slot].key, p_key, MAX_PATH);
        strncpy(conf->conf_data[slot].val, p_val, MAX_PATH);
        slot++;
    }

    conf->real_conf_num = slot;
    return 0;
}

void _trim(char *str)
{
    if (str == NULL)
	return;

    char *begin, *end;

    begin = str;
    while (isspace(*begin))
	++begin;

    end = begin + strlen(begin);
    if (end > begin)
	while (isspace(*--end))
		*end = '\0';

    memmove(str, begin, end - begin + 1);
}

int sl_conf_getstr(const sl_conf_data_t *conf, const char *conf_name, 
                   char *conf_value, size_t conf_size, const char *default_value)
{
    if (conf == NULL || conf_name == NULL || conf_value == NULL)
	return -1;

    int i;
    for (i = 0; i < conf->real_conf_num; i++)
    {
        if (strcasecmp(conf->conf_data[i].key, conf_name) == 0)
	{
 	    strncpy(conf_value, conf->conf_data[i].val, conf_size);
            return 0;
        }
    }

    if (default_value != NULL)
        strncpy(conf_value, default_value, conf_size);

    return 0;
}

int sl_conf_getint(const sl_conf_data_t *conf, const char *conf_name,
                   int *conf_value, const int *default_value)
{
    if (conf == NULL || conf_name == NULL || conf_value == NULL)
        return -1;
    
    int i;
    for (i = 0; i < conf->real_conf_num; i++)
    {
        if (strcasecmp(conf->conf_data[i].key, conf_name) == 0)
	{
	    *conf_value = atoi(conf->conf_data[i].val);
            return 0;
        }
    }

    if (default_value != NULL)
	*conf_value = *default_value;

    return 0;
}


