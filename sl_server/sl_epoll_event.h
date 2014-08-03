#ifndef _SL_EPOLL_EVENT_H_
#define _SL_EPOLL_EVENT_H_

int epoll_init(int max_evts);
int epoll_listen(int listen_fd);

void* epoll_master(void *);
void* epoll_worker(void *);

int epoll_destroy();


#endif
