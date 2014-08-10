#include "sl_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    sl_client_t *client = sl_client_create("default");
    

    sl_client_connect(client, "127.0.0.1", 8080);

    void *write_buf = sl_client_get_write_buf(client);
    strncpy(write_buf, "hi, pengcheng", 14);

    sl_client_talk_server(client);

    void *read_buf = sl_client_get_read_buf(client);
    printf("%s\n", (char*)read_buf);

    return 0;
}
