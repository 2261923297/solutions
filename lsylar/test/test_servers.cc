#include "servers.h"
#include "Socket.h"
#include "Log.h"
#include <thread>


void test_base() {

	reactor_t reactor;
	init_reactor(&reactor);

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
	init_reactor(&reactor);

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

int main() {
//	test_base();

	test_conn_1000k();
	return 0;
}
