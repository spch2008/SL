#include "sl_server_epool.h"
#include "sl_server_event.h"
#include "sl_server_queue.h"
#include <sys/epool.h>


static int epoll_fd;
static int max_events;

static int epool_add(int fd, int data)
{
    struct epoll_event event;

    memset(&event, 0, sizeof(struct epoll_event));
    event.events |= EPOLLIN;
    event.data.fd = data;
    
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    if (ret == -1)
	return -1;

    return 0;
}

static int epoll_produce(sl_server_t *server)
{
    int i, nfds;
    struct epoll_event *events = NULL;
    struct sockaddr addr;
 
    events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * max_events);
    if (events == NULL)
	return -1;

    for ( ; ; ) {
	nfds = epoll_wait(epoll_fd, events, max_events, -1);
        
        for (i = 0; i < nfds; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                epoll_del(events[i].data.fd);
		close(events[i].data.fd);

	    } else if (events[i].data.fd == 0) {
		while (1) {
		    addrlen = sizeof(struct sockaddr);
                    fd = accept(listen_fd, &addr, &addrlen);
                    if (fd == -1) {
	                continue;
 	 	    }

	            epoll_add(fd, fd);
    		}
            } else {

                sl_server_queue_put_item(server, events[i].data.fd);
            }
    }
}

static int epoll_consume(sl_server_t *server)
{
    int fd, res;

    res  = sl_server_queue_get_item(server, &fd);
    if (res == -1)
	return -1;

    server->default_callback();
}

int epoll_init(int m_evt)
{   
    int res;
    
    max_events = m_evt;
    if (max_events < 1)
	return -1;

    epoll_fd = epoll_create(m_evt);
    if (epoll_fd == -1)
	return -1;

    res = sl_server_queue_init(100);
    if (res == -1)
	return -1;

    return 0;
}


int epoll_listen(int fd)
{
    return epoll_add(fd, 0);
}


void *epoll_master(void *param)
{
    sl_server_tdata_t *tdata = (sl_server_tdata_t*)param;
    sl_server_t *server      = tdata->parent;

    epoll_produce(server);
}


void *epoll_worker(void *param)
{

    sl_server_tdata_t *tdata = (sl_server_tdata_t*)param;
    sl_server_t *server      = tdata->parent;


    epoll_consume(server);
        
}


