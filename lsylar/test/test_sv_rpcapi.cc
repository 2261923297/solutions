#include "sv_rpcapi.h"
#include "servers.h"
#include "reqpool.h"
#include "Log.h"

#include <unistd.h>
#include <thread>
#include <errno.h>
#include <mutex>

typedef struct __st_rpcapi_reqpool_cb_context
{
	int epfd;
	int fd;

} rpcapi_reqpool_cb_context_t;

bool reqpool_parser_cb(void* content, size_t& len, void* cb_context) {
    bool rt = true;
	rpcapi_reqpool_cb_context_t* ctx = 
			(rpcapi_reqpool_cb_context_t*)cb_context;

    INFO_SYS << "respond: " 
			<< std::string((const char*)content, 0, len);
    std::string cp_words 
		= " >>> after compute in pw server and parser in cb";
    memcpy((uint8_t*)content + len
            , cp_words.c_str()
            , cp_words.size());
    len = len + cp_words.size();

	// event move to send
	//
#if 1
	epoll_event ev;
	ev.data.fd = ctx->fd;
	ev.events = EPOLLOUT | EPOLLET;
	int ret = epoll_ctl(ctx->epfd
			, EPOLL_CTL_MOD
			, ctx->fd
			, &ev);
	if(ret == SOCKET_ERROR)
	{
		ERROR_SYS << "epoll add: " << strerror(errno);
	}

#else 
	int ret = ::send(ctx->fd, content, len, 0);
	if(ret == SOCKET_ERROR) {
		ERROR_SYS << "send " << strerror(errno); 
	} else {
		INFO_SYS << "n_send: " << ret;
	}
//	*/
#endif
	free(ctx);

    return rt;
}


int rpcapi_recv_callback(int fd, void* args) 
{
	reactor_t* reactor = (reactor_t*)args;
	fd_item_t* fd_item = get_fd_item(reactor, fd);

	// recv msg
	memset(fd_item->rbuffer, 0, SR_BUFFER_LEN);
	fd_item->rlen = SR_BUFFER_LEN;
	tt::system::Socket s(fd);
	s.recv(fd_item->rbuffer, fd_item->rlen);
	
	if(fd_item->rlen <= 0) {
		if(fd_item->rlen == SOCKET_ERROR) {
			ERROR_SYS << "cant^t recv err_desc: "
					<< strerror(errno);
		} else {

		}
		epoll_ctl(reactor->work_epfd
				, EPOLL_CTL_DEL
				, fd
				, nullptr);
		del_fd(reactor, fd);
		return fd_item->rlen;
	} else {
		// Socket 析构会 close fd
		s.set_sockfd(0);
	}
	DEBUG_SYS << "what recv: "
			<< std::string((char*)fd_item->rbuffer
							, 0, fd_item->rlen);

#if 1
	// rpcapi send req to server
	rpcapi_reqpool_cb_context_t* ctx = 
		(rpcapi_reqpool_cb_context_t*)malloc(sizeof(rpcapi_reqpool_cb_context_t));
	ctx->epfd = reactor->work_epfd;
	ctx->fd = fd;
//	tt::system::Socket::ptr client_sock(
//			new tt::system::Socket(fd_item->fd));
	asynreq_commit(
			tt::profession::ServerRpcApi::s_reqpool
			, &reqpool_parser_cb
			, ctx 
			, fd_item->rbuffer
			, fd_item->rlen
			, fd_item->sbuffer
			, fd_item->slen);
#else
	// echo server
	memcpy(fd_item->sbuffer
			, fd_item->rbuffer
			, fd_item->rlen);
	fd_item->slen = fd_item->rlen;
	
    epoll_event ev;
    ev.data.fd = fd;
    ev.events=EPOLLOUT | EPOLLET;
    int ret = epoll_ctl(
        reactor->work_epfd
        , EPOLL_CTL_MOD
        , fd
        , &ev);
#endif

	return 0;
}

int sv_rpcapi_init_fd_item(fd_item_t* item) {
	if(item == nullptr) {
		ERROR_SYS << "param reactor is null";
		return -1;
	}
	item->fd = 0;
	item->event = 0;

	item->recv_cb = &rpcapi_recv_callback;
	item->send_cb = &send_callback;
	item->accept_cb = &accept_callback;

	memset(item->sbuffer, 0, SR_BUFFER_LEN);
	memset(item->rbuffer, 0, SR_BUFFER_LEN);
	item->slen = 0;
	item->rlen = 0;

	return 0;
}

fd_item_block_t* sv_rpcapi_alloc_fd_item_block(int index) {
	fd_item_block_t* rt = nullptr;
	DEBUG_SYS << "alloc block";
	// malloc fd_item_block
	rt = (fd_item_block_t*)malloc(sizeof(fd_item_block_t));
	if(rt == nullptr) {
		ERROR_SYS << "memory is run out";
		return rt;
	}
	// malloc fd_item * fd_item_block_size;
	rt->items = 
		(fd_item_t*)malloc(
			sizeof(fd_item_t) * FD_ITEM_BLOCK_SIZE);
	if(rt->items == nullptr)
	{
		ERROR_SYS << "memory is run out";
		free(rt);
		rt = nullptr;
		return rt;
	}
	// init fd_item
	for(size_t i = 0; i < FD_ITEM_BLOCK_SIZE; i++) 
	{
		sv_rpcapi_init_fd_item(rt->items + i);
	}
	rt->index = index;
	return rt;
}


namespace tt
{
namespace profession
{
ServerRpcApi::ServerRpcApi() 
{ 
	m_reactor = nullptr;
	init(); 
}

asynreq_context* ServerRpcApi::GetReqpool() 
{ 
	if(s_reqpool == nullptr)
	{
		s_reqpool = (asynreq_context*)malloc(sizeof(asynreq_context));
		if(s_reqpool == nullptr) {
			ERROR_SYS << "memory is runout";
			return nullptr;
		} else {
			INFO_SYS << "reqpool_addr: " << s_reqpool;
			asynreq_context_init(s_reqpool);
			INFO_SYS << "reqpool_addr: " << s_reqpool;
		}
	}
	return s_reqpool; 
}

bool ServerRpcApi::init()
{
	// check private member
	if(m_reactor != nullptr) 
	{
		destory_reactor(m_reactor);
		m_reactor = nullptr;
	}

	// alloc memeory
//	GetReqpool();
	m_reactor = (reactor_t*)malloc(sizeof(reactor_t));
	if(m_reactor == nullptr)
	{
		ERROR_SYS << "memeory is run out";
		return false;
	}

	// init private member
	init_reactor(m_reactor
			, sv_rpcapi_alloc_fd_item_block);

	INFO_SYS << "Server RpcApi init complete";
	return true;
}

bool ServerRpcApi::destory()
{
	// destory private member
	destory_reactor(m_reactor);

	// set nullptr
	free(m_reactor);
	m_reactor = nullptr;
	INFO_SYS << "Server RpcApi stop !";

	return true;
}

bool ServerRpcApi::run()
{

	INFO_SYS << "Server RpcApi start !";
	// create thread 
	std::thread thrd_listen(listen_loop, m_reactor);
	std::thread thrd_work(work_loop, m_reactor);

	// init reqpool req IP:8888 
	asynreq_context_init(s_reqpool, IP, 8888);

	// init listen socket listen 192.168.43.110:7788
	tt::system::Socket listenfd;
	listenfd.init_tcp("192.168.43.110", 7788);
	listenfd.listen(1024 * 1024);
	// add listen socket
	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd.get_sockfd();
	epoll_ctl(m_reactor->listen_epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
	add_fd(m_reactor, listenfd.get_sockfd());

	// thread join
	thrd_listen.join();
	thrd_work.join();	

	return true;
}



} // namespace profession
} // namespace tt


int main() 
{

	tt::profession::ServerRpcApi sra;
	TT_DEBUG << "reqpool_addr: " << tt::profession::ServerRpcApi::GetReqpool();
	sra.run();
	while(1)
	{
		sleep(1);
	}
	return 0;
}

