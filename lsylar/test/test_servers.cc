#include "servers.h"
#include "util.h"
#include "Socket.h"
#include "Log.h"
#include <thread>

/*
*/

#define SR_DEBUG_OPEN 0

int udp_recv_cb(int fd, void* args)
{
	
	reactor_t* reactor = (reactor_t*)args;
	fd_item_t* fd_item = get_fd_item(reactor, fd);

	//TT_DEBUG << "recv_cb" << str_val(fd_item->fd);
	// recv msg
	memset(fd_item->rbuffer, 0, SR_BUFFER_LEN);
	fd_item->rlen = SR_BUFFER_LEN;
	tt::system::Socket s(fd, false);
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
//		close(fd_item->fd);
		return fd_item->rlen;
	} else {
		// Socket 析构会 close fd
		s.set_sockfd(0);
	}
#if SR_DEBUG_OPEN
	DEBUG_SYS <<  str_val(fd_item->rlen);
	DEBUG_SYS << "what recv: "
			<< std::string((char*)fd_item->rbuffer
							, 0, fd_item->rlen);
#endif
	// echo server
	memcpy(fd_item->sbuffer
			, fd_item->rbuffer
			, fd_item->rlen);
	fd_item->slen = fd_item->rlen;

	// event move to send
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLOUT | EPOLLET;
	epoll_ctl(reactor->work_epfd
			, EPOLL_CTL_MOD
			, fd
			, &ev);

	return 0;
}

int udp_send_cb(int fd, void* args)
{

	reactor_t* reactor = (reactor_t*)args;
	fd_item_t* fd_item = get_fd_item(reactor, fd);

	if(fd_item->slen == 0) { return -2; }
	tt::system::Socket s(fd, false);
	s.send(fd_item->sbuffer, fd_item->slen);
#if SR_DEBUG_OPEN
	TT_DEBUG << "send: " << std::string(
		(char*)fd_item->sbuffer
		, 0
		, fd_item->slen
	);
#endif
	if(fd_item->slen <= 0) {
		epoll_ctl(reactor->work_epfd, EPOLL_CTL_DEL, fd, nullptr);
		del_fd(reactor, fd);
		return -1;
	} else {
		s.set_sockfd(0);
	}
	fd_item->slen = 0;
	// modify recv
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(reactor->work_epfd, EPOLL_CTL_MOD, fd, &ev);

	return 0;
}

void test_echo_server() {
	TT_DEBUG << "echo epoll server";
	reactor_t reactor;
	init_reactor(&reactor);

	std::thread listen_thrd(listen_loop, &reactor);
	std::thread work_thrd(work_loop, &reactor);
	
	// add udp echo server
	const char* udpsv_ip = "192.168.90.1";
	uint16_t udpsv_port = 9999;
	reactor_add_udp_server(
			&reactor
			, udpsv_ip
			, udpsv_port
			, udp_recv_cb
			, udp_send_cb);

	// add n_listener 
	int n_listener = 10;
	for(int i = 0; i < n_listener; i++)
	{
		reactor_add_listener(&reactor, PW_SERVER_IP, 8800 + i);
	}
	listen_thrd.join();
	work_thrd.join();
}


void test_listen_api() 
{
	reactor_t reactor;
	init_reactor(&reactor, alloc_fd_item_block);

	std::thread listen_thrd(listen_loop, &reactor);
	std::thread work_thrd(work_loop, &reactor);

	tt::system::Socket listenfd;
	listenfd.init_tcp();
	listenfd.listen(SOMAXCONN);

	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd.get_sockfd();
	epoll_ctl(reactor.listen_epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
	add_fd(&reactor, listenfd.get_sockfd());

	listen_thrd.join();
	work_thrd.join();

}
void test_conn_1000k() {
	reactor_t reactor;
	init_reactor(&reactor, alloc_fd_item_block);

	TT_DEBUG << SOMAXCONN << " " << 1024 * 1024;
	tt::system::Socket listenfd;
	listenfd.init_tcp();
	listenfd.listen(1024 * 1024);

	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd.get_sockfd();
	epoll_ctl(reactor.listen_epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
	add_fd(&reactor, listenfd.get_sockfd());

	listen_loop(&reactor);
}

int udp_echo_server_recv();
int main() {
	test_echo_server();
//	test_conn_1000k();
	return 0;
}





