#include "reqpool.h"
#include "Log.h"
#include "Socket.h"
#include "util.h"

#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdio.h>

bool asynreq_context_init(
    asynreq_context* asynreq_ctx
	, const char* sv_ip
	, uint16_t sv_port
) 
{
    bool rt = true;
    // check param
    if(asynreq_ctx == nullptr) 
    {
        TT_DEBUG << "asyncreq_ctx is null";
        return false;
    } 

	// init member
	memcpy(asynreq_ctx->server_ip, sv_ip, strlen(sv_ip));
	asynreq_ctx->server_port = sv_port;
    // create epoll
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
        ::close(epfd);
        rt = false;
        return rt;
    }
	INFO_SYS << "asynchronous request pool will send request to "
			<< sv_ip << ":"
			<< sv_port;
    return rt;
}



bool asynreq_commit_udp(
    asynreq_context* asynreq_ctx
	, asynreq_result_cb _cb
	, void* _cb_context
    , const void* send_buffer
    , size_t send_len
	, void* result_buffer
	, size_t& result_buffer_len
)
{
    bool rt = true;
    // send protocol
    size_t n_send = send_len;
	// init tcp
    tt::system::Socket::ptr req_sock(
			new tt::system::Socket);
    req_sock->init_udp(
			"0.0.0.0"
			, 0);
	tt::system::IPAddress::ptr 
		remote(new tt::system::IPv4Address);
	remote->set_ip(asynreq_ctx->server_ip);
	remote->set_port(asynreq_ctx->server_port);
	req_sock->set_remote_addr(remote);

//	TT_DEBUG << "will send";
	// send
    rt = req_sock->send(
        send_buffer
        , send_len
    );
    // init eparg
    eparg_t *eparg = nullptr;
    eparg = new eparg_t(
			req_sock
			, _cb
			, _cb_context
			, result_buffer
			, result_buffer_len
			);
    if(eparg == nullptr) {
        ERROR_SYS << "memory is run out" 
                << strerror(errno);
        return false;
    }
    // epoll add
    epoll_event ev;
    ev.data.ptr = eparg;
    ev.events=EPOLLIN | EPOLLET;
    int ret = epoll_ctl(
        asynreq_ctx->epfd
        , EPOLL_CTL_ADD
        , req_sock->get_sockfd()
        , &ev);
    if(ret == SOCKET_ERROR) {
        ERROR_SYS << "reqpool commit->epoll_ctl: " 
                << strerror(errno);
        rt = false;
    } else {

    }
    return rt;
}

bool asynreq_commit(
    asynreq_context* asynreq_ctx
	, asynreq_result_cb _cb
	, void* _cb_context
    , const void* send_buffer
    , size_t send_len
	, void* result_buffer
	, size_t& result_buffer_len
)
{
    bool rt = true;
    // send protocol
    size_t n_send = send_len;
	// init tcp
    tt::system::Socket::ptr req_sock(
			new tt::system::Socket);
    req_sock->init_tcp("0.0.0.0", 0);
    if(!req_sock->connect(
				asynreq_ctx->server_ip
				, asynreq_ctx->server_port))
    {
        TT_DEBUG << "cant^t connect!";
        return false;
    }
	// send
    rt = req_sock->send(
        send_buffer
        , send_len
    );
    // init eparg
    eparg_t *eparg = nullptr;
    eparg = new eparg_t(
			req_sock
			, _cb
			, _cb_context
			, result_buffer
			, result_buffer_len
			);
    if(eparg == nullptr) {
        ERROR_SYS << "memory is run out" 
                << strerror(errno);
        return false;
    }
    // epoll add
    epoll_event ev;
    ev.data.ptr = eparg;
    ev.events=EPOLLIN | EPOLLET;
    int ret = epoll_ctl(
        asynreq_ctx->epfd
        , EPOLL_CTL_ADD
        , req_sock->get_sockfd()
        , &ev);
    if(ret == SOCKET_ERROR) {
        ERROR_SYS << "reqpool commit->epoll_ctl: " 
                << strerror(errno);
        rt = false;
    } else {

    }
    return rt;
}

static uint64_t ltime = 0;
static uint64_t rcount = 0;
void* asynreq_callback(void* cbarg) {
    TT_DEBUG << "asynreq_callback: ";
    asynreq_context* req_ctx = 
        (asynreq_context*)cbarg;
    const int event_size = 100;
    epoll_event cur, events[event_size];

    int nfds = 0;
    eparg_t* eparg = nullptr;
    while(1) {
        // epoll_wait
        nfds = epoll_wait(req_ctx->epfd, events, event_size, -1);
        if(nfds == -1) {
            TT_DEBUG << "epoll wait err"
                    << strerror(errno);
            return NULL;
        }
//        TT_DEBUG << "nfds: " << nfds;
        for(int i = 0; i < nfds; i++) {
            eparg = (eparg_t*)events[i].data.ptr;
            memset(eparg->result_buffer, 0, eparg->result_buffer_len);
            // recv
            if(!eparg->req_sender->recv(
						eparg->result_buffer
						, eparg->result_buffer_len)) {
                ERROR_SYS << "eparg->req_sender cant^t recv data"
                        << eparg->req_sender.get();
                continue;
            }
//			TT_DEBUG << "recv_time_us: " << cur_time_us();

		    // 将fd 从epoll中移除
            epoll_ctl(
                req_ctx->epfd
                , EPOLL_CTL_DEL
                , eparg->req_sender->get_sockfd()
                , 0);
            // parser and other
            eparg->cb(
					eparg->result_buffer
					, eparg->result_buffer_len
					, eparg->cb_context);
            delete eparg;
        }
		rcount += nfds;
#if 1
		if(rcount % 1024 == 0)
		{
			uint64_t ntime = cur_time_us();
			ltime = ntime - ltime;

			TT_DEBUG << str_val(rcount)
				<< " cost: " << ltime << "us";
			ltime = ntime;
		}
#endif
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
