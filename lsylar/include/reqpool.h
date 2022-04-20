#include <stdio.h>
#include <thread>

#include "Socket.h"
/* commit
    socket
    connect server
    encode --> redis/mysql/dns
    fd --> epoll 
*/

/* thread_callback
    while(1) {
        epoll_wait();
        recv();
        parser();
        fd --> delete
    }

*/

/* init 
    epoll_create
    pthread_create
*/

/* destory
    close(epfd)
    pthread_destory();
*/

typedef bool (*asynreq_result_cb)(const void* content, size_t& len);




struct eparg_t {
    int fd;
    asynreq_result_cb cb;
};


struct asynreq_context {
    int epfd;
    pthread_t thid;
    
};

void* asynreq_callback(void* cbarg);
bool asynreq_commit(asynreq_context* asyncreq_ctx
                    , tt::system::Socket::ptr client_sock);

bool asynreq_context_init(asynreq_context* asynreq_ctx);
bool asynreq_context_destory(asynreq_context* asynreq_ctx);



// listenfd 用水平触发