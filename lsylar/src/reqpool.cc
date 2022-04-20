#include "reqpool.h"
#include "Log.h"
#include "Socket.h"
#include "util.h"
#include <sys/epoll.h>
#include <errno.h>

bool pt_recv(const void* content, size_t& len) {
    bool rt = true;

    TT_DEBUG << std::string((const char*)content, len);

    return rt;
}

bool asynreq_context_init(
    asynreq_context* asynreq_ctx
) 
{
    bool rt = true;
    if(asynreq_ctx == nullptr) 
    {
        TT_DEBUG << "asyncreq_ctx is null";
        return false;
    } 

    int epfd = epoll_create(1);
    if (epfd < 0) 
    {
        TT_DEBUG << "can^t epoll_create!" 
                << strerror(errno);
        rt = false;
        return rt;
    }
    asynreq_ctx->epfd = epfd;

    // pthread_create
    int ret = 
        pthread_create(
            &asynreq_ctx->thid
             , NULL
            , asynreq_callback
            , asynreq_ctx);
    if(ret != 0) 
    {
        TT_DEBUG << "pthread_create error!"
                << strerror(errno);
        close(epfd);
        rt = false;
        return rt;
    }
    
    
    return rt;
}


bool asynreq_commit(
    asynreq_context* asynreq_ctx
    , tt::system::Socket::ptr client_sock
)
{
    bool rt = true;
    // send protocol
    std::string say_words = "hello, req poll!";
    size_t n_send = say_words.size();
    client_sock->send(
        say_words.c_str()
        , n_send);
    // epoll add
    eparg_t *eparg =
        (eparg_t*) malloc(sizeof(eparg_t));
    eparg->fd = client_sock->get_sockfd();
    eparg->cb = &pt_recv;

    epoll_event ev;
    ev.data.ptr = eparg;
    ev.events=EPOLLIN | EPOLLET;

    int ret = epoll_ctl(
        asynreq_ctx->epfd
        , EPOLL_CTL_ADD
        , client_sock->get_sockfd()
        , &ev);
    if(ret == SOCKET_ERROR) {
        TT_DEBUG << "epoll_ctl: " 
                << strerror(errno);
    } else {
        TT_DEBUG << "commit sock = "  << client_sock->get_sockfd()
                << " to epfd = " << asynreq_ctx->epfd;
    }
//    asynreq_callback((void*)asynreq_ctx);
    return rt;
    
}

void* asynreq_callback(void* cbarg) {
    TT_DEBUG << "asynreq_callback: ";
    asynreq_context* req_ctx = 
        (asynreq_context*)cbarg;
    epoll_event cur, events[100];

    const size_t bf_size = 1024 * 4;  
    uint8_t tmp_buffer[bf_size] = { 0 };

    int nfds = 0;
    eparg_t* eparg = nullptr;
    while(1) {
        // epoll_wait
        nfds = epoll_wait(req_ctx->epfd, events, 100, 500);
        if(nfds == -1) {
            TT_DEBUG << "epoll wait err"
                    << strerror(errno);
            return NULL;
        }

        for(int i = 0; i < nfds; i++) {
            eparg = (eparg_t*)events[i].data.ptr;
            memset(tmp_buffer, 0, bf_size);
            // recv
            size_t n_recv = 
                ::recv(
                    eparg->fd
                    , tmp_buffer
                    , bf_size
                    , 0);
            if(n_recv == SOCKET_ERROR) {
                TT_DEBUG << "err recv" 
                        << strerror(errno);
                continue;
            }
            // parser
            eparg->cb(tmp_buffer, n_recv);
            
            // delete fd
            epoll_ctl(
                req_ctx->epfd
                , EPOLL_CTL_DEL
                , eparg->fd
                , 0);
            free(eparg);
        }
    }
    return nullptr;
}

bool asynreq_context_destory(
    asynreq_context* asynreq_ctx
)
{
    bool rt = true;
    // clise epfd
    close(asynreq_ctx->epfd);
    // cancel thid
    pthread_cancel(asynreq_ctx->thid);

    return rt;
}
