#include "servers.h"
#include "Log.h"
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <sys/socket.h>

int init_fd_item(fd_item_t* item) {
	if(item == nullptr) {
		ERROR_SYS << "param reactor is null";
		return -1;
	}
	
	item->fd = 0;
	item->event = 0;

	item->recv_cb = &recv_callback;
	item->send_cb = &send_callback;
	item->accept_cb = &accept_callback;

	memset(item->sbuffer, 0, SR_BUFFER_LEN);
	memset(item->rbuffer, 0, SR_BUFFER_LEN);
	item->slen = 0;
	item->rlen = 0;

	return 0;
}

fd_item_block_t* alloc_fd_item_block(int index) {
	fd_item_block_t* rt = nullptr;
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
	for(size_t i = 0; i < FD_ITEM_BLOCK_SIZE; i++) {
		init_fd_item(rt->items + i);
	}
	rt->index = index;
	return rt;
}

void free_fd_item_block_content(fd_item_block_t* block) {
	if(block == nullptr) { return; }
	if(block->items != nullptr)
		free(block->items);
	block->items = nullptr;
}

int init_reactor(reactor_t *reactor)
{
	if(reactor == nullptr) {
		ERROR_SYS << "param reactor is null";
		return -1;
	}
	reactor->listen_epfd = epoll_create(1);
	if(reactor->listen_epfd == -1) {
		ERROR_SYS << "can^t epoll_create"
				<< strerror(errno);
		return -1;
	}
	reactor->work_epfd = epoll_create(1);
	if(reactor->work_epfd == -1) {
		ERROR_SYS << "can^t epoll_create"
				<< strerror(errno);
		return -1;
	}
	reactor->nfd = 0;
	reactor->fd_item_blocks.push_back(alloc_fd_item_block(0));
	return 0;
}

int destory_reactor(reactor_t* reactor) 
{
	close(reactor->listen_epfd);
	reactor->listen_epfd = 0;
	close(reactor->work_epfd);
	reactor->work_epfd = 0;
	reactor->nfd = 0;

	for(auto it : reactor->fd_item_blocks) {
		free_fd_item_block_content(it);
		free(it);
	}
	reactor->fd_item_blocks.clear();
	return 0;
}


int add_fd(reactor_t* reactor, int fd)
{
	int block_num = (fd / FD_ITEM_BLOCK_SIZE) + 1;
	int block_index = block_num - 1;
	int block_offset = fd % FD_ITEM_BLOCK_SIZE;
	
	if(fd % FD_ITEM_BLOCK_SIZE == 0) {
		DEBUG_SYS << "Connection over " << reactor->nfd
				<< " fd = " << fd;
	}
	for(size_t i = reactor->fd_item_blocks.size()
			; i < block_num
			; i++)
	{
		INFO_SYS << "add one block";
		reactor->
			fd_item_blocks.push_back
			(alloc_fd_item_block(i));
	}
	reactor->fd_item_blocks[block_index]->items[block_offset].fd = fd;

	reactor->nfd++; 
	
	return 0;
}

int del_fd(reactor_t* reactor, int fd) 
{
	int block_index = fd / FD_ITEM_BLOCK_SIZE;
	int block_offset = fd % FD_ITEM_BLOCK_SIZE;
	if(block_index >= reactor->fd_item_blocks.size()) { return -1; }
	reactor->fd_item_blocks[block_index]->items[block_offset].fd = 0;
	reactor->nfd--;

	return 0;
}

fd_item_t* get_fd_item(reactor_t* reactor, int fd)
{
	fd_item_t* rt = nullptr;
	int block_index = fd / FD_ITEM_BLOCK_SIZE;
	int block_offset = fd % FD_ITEM_BLOCK_SIZE;
	if(block_index >= reactor->fd_item_blocks.size()) {
		ERROR_SYS << "there is no fd = " << fd << "!";
	}
	rt = reactor->fd_item_blocks[block_index]->items + block_offset;
	if(rt->fd == 0) {
		DEBUG_SYS << "fd = " << fd << " have been deleted! ";
	}
	return rt;
}

int listen_loop(reactor_t* reactor) {
	if(reactor == nullptr) { 
		ERROR_SYS << "param error";
		return -1;
	}
	if(reactor->listen_epfd <= 0) {
		ERROR_SYS << "param error";
		return -1;
	}
	epoll_event events[EPOLL_LISTEN_EVENT_SIZE] = { 0 };
	while(1) {
		int n_ready = epoll_wait(
			reactor->listen_epfd
			, events
			, EPOLL_LISTEN_EVENT_SIZE
			, -1);
		if(n_ready < 0) {
			ERROR_SYS << "epoll_wait error"
					<< strerror(errno);
			return -1;
		}
		for(int i = 0; i < n_ready; i++) 
		{
			int fd = events[i].data.fd;
			fd_item_t* fd_item = get_fd_item(reactor, fd);
			fd_item->accept_cb(fd, reactor);
		}

	}
	return 0;
}

int work_loop(reactor_t* reactor)
{
	if(reactor == nullptr) { 
		ERROR_SYS << "param error";
		return -1;
	}
	if(reactor->work_epfd <= 0) {
		ERROR_SYS << "param error";
		return -1;
	}
	epoll_event events[EPOLL_LISTEN_EVENT_SIZE] = { 0 };
	while(1) {
		int n_ready = epoll_wait(
			reactor->work_epfd
			, events
			, EPOLL_WORK_EVENT_SIZE
			, -1);
		if(n_ready < 0) {
			ERROR_SYS << "epoll_wait error"
					<< strerror(errno);
			return -1;
		}
		for(int i = 0; i < n_ready; i++) 
		{
			int fd = events[i].data.fd;
			fd_item_t* fd_item = get_fd_item(reactor, fd);
			if(events[i].events & EPOLLIN) {
				fd_item->recv_cb(fd, reactor);
			}
			if(events[i].events & EPOLLOUT) {
				fd_item->send_cb(fd, reactor);
			}
		}

	}
	return 0;
}

int recv_callback(int fd, void* args) {
	reactor_t* reactor = (reactor_t*)args;
	fd_item_t* fd_item = get_fd_item(reactor, fd);

	memset(fd_item->rbuffer, 0, SR_BUFFER_LEN);
	fd_item->rlen = ::recv(fd, fd_item->rbuffer, SR_BUFFER_LEN, 0);
	if(fd_item->rlen <= 0) {
		ERROR_SYS << "cant^t recv err_desc: "
				<< strerror(errno);
		epoll_ctl(reactor->work_epfd, EPOLL_CTL_DEL, fd, nullptr);
		close(fd);
		del_fd(reactor, fd);
		return fd_item->rlen;
	}

	// echo server
	memcpy(fd_item->sbuffer, fd_item->rbuffer, fd_item->rlen);
	fd_item->slen = fd_item->rlen;
	DEBUG_SYS << "what recv: "
			<< std::string((char*)fd_item->rbuffer, 0, fd_item->rlen);
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLOUT | EPOLLET;
	epoll_ctl(reactor->work_epfd, EPOLL_CTL_MOD, fd, &ev);
	return 0;
}

int send_callback(int fd, void* args) {
	reactor_t* reactor = (reactor_t*)args;
	fd_item_t* fd_item = get_fd_item(reactor, fd);

	memset(fd_item->rbuffer, 0, SR_BUFFER_LEN);
	fd_item->slen = ::send(fd, fd_item->sbuffer, fd_item->slen, 0);
	if(fd_item->slen <= 0) {
		ERROR_SYS << "cant^t recv";
		epoll_ctl(reactor->work_epfd, EPOLL_CTL_DEL, fd, nullptr);
		close(fd);
		del_fd(reactor, fd);
	}

	// modify recv
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(reactor->work_epfd, EPOLL_CTL_MOD, fd, &ev);

	return 0;
}

int accept_callback(int fd, void* args) {
	reactor_t* reactor = (reactor_t*)args;

	// accept
	sockaddr addr;
    socklen_t len;
    int client = ::accept(fd, &addr, &len);
	if(client == 0) {
		ERROR_SYS << "cant^t accept";
		epoll_ctl(reactor->work_epfd, EPOLL_CTL_DEL, fd, nullptr);
		close(fd);
		del_fd(reactor, fd);
		return -1;
	}

	// add to work_epfd
	epoll_event ev;
    ev.data.fd = client;
    ev.events = EPOLLIN | EPOLLET;
    int ret = epoll_ctl(
        reactor->work_epfd
        , EPOLL_CTL_ADD
        , client
        , &ev);
	add_fd(reactor, client);
	return 0;
}


