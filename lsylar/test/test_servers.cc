#include "servers.h"
#include "Socket.h"
#include "Log.h"
#include <thread>

/*
*/



void test_echo_server() {
	TT_DEBUG << "echo epoll server";
	reactor_t reactor;
	init_reactor(&reactor);

	std::thread listen_thrd(listen_loop, &reactor);
	std::thread work_thrd(work_loop, &reactor);

/*
	tt::system::Socket listenfd;
	const char* listen_ip = "192.168.90.1";
	uint16_t    listen_port = 8888;
	TT_DEBUG << "listen_ip: " << listen_ip
			<< ", listen_port: " << listen_port;
	listenfd.init_tcp(listen_ip, listen_port);
	listenfd.listen(1024 * 1024);

	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd.get_sockfd();
	epoll_ctl(reactor.listen_epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
	add_fd(&reactor, listenfd.get_sockfd());
*/
	// add n_listener 
	int n_listener = 100;
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
