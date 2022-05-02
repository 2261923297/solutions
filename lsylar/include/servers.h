#pragma once
#include <sys/epoll.h>
#include <string>
#include <stdint.h>
#include <vector>

static const int SR_BUFFER_LEN = 512;
static const int FD_ITEM_BLOCK_SIZE = 1024;

static const int EPOLL_WORK_EVENT_SIZE = 1024;
static const int EPOLL_LISTEN_EVENT_SIZE = 1024;

typedef int (*NET_EVENT_CB)(int fd, void* args);

#define PW_SERVER_IP "192.168.90.1"
#define PW_SERVER_PORT 8888

struct __st_reactor;

typedef struct __st_fd_item {
    int fd;
    int sock_type;

    NET_EVENT_CB recv_cb;
    NET_EVENT_CB send_cb;
    NET_EVENT_CB accept_cb;

    uint8_t sbuffer[SR_BUFFER_LEN];
    size_t slen;

    uint8_t rbuffer[SR_BUFFER_LEN];
    size_t rlen;

} fd_item_t;

typedef struct __st_fd_item_block {
    fd_item_t* items;
    int index;
} fd_item_block_t;


typedef fd_item_block_t* (*BLOCK_ALLOCATOR_FN)(int);
typedef void (*BLOCK_COTENT_DEALLOCATOR_FN)(fd_item_block_t*);


// 自己创建 reactor 管理fd_item原因: 减少对 epoll 的依赖
typedef struct __st_reactor {

    int listen_epfd;
    int work_epfd;

    // memory ctl
    BLOCK_ALLOCATOR_FN              block_allocator;
    BLOCK_COTENT_DEALLOCATOR_FN     block_content_deallocator;

    // vector.size() 并不是真正的fd数量
    std::vector<fd_item_block_t*> fd_item_blocks;

    size_t nfd;
} reactor_t;

fd_item_block_t* alloc_fd_item_block(int index);
void free_fd_item_block_content(fd_item_block_t* block);

int add_fd(reactor_t* reactor, int fd);
int del_fd(reactor_t* reactor, int fd);
fd_item_t* get_fd_item(reactor_t* reactor, int fd);


// echo server
int init_reactor(reactor_t *reactor
    , BLOCK_ALLOCATOR_FN alloc_fd_item_block_fn = &alloc_fd_item_block
    , BLOCK_COTENT_DEALLOCATOR_FN block_content_deallocator = &free_fd_item_block_content);
int destory_reactor(reactor_t* reactor);

// recv and send
int work_loop(reactor_t* reactor);
// only listen
int listen_loop(reactor_t* reactor);
/*
int reactor_add_udp_server(
		reactor_t* reactor
		, const char* ip
		, uint16_t port
		, NET_EVENT_CB recv_cb
		, NET_EVENT_CB send_cb
)
{
	tt::system::Socket udp_sock;
	udp_sock.init_udp(ip, port);

	fd_item_t* fd_item = get_fd_item(udp_sock.get_sockfd());
	fd_item->fd = udp_sock.get_sockfd();
	fd_item->recv_cb = recv_cb;
	fd_item->send_cv = send_cb;

	// add to work_epfd
	epoll_event ev;
    ev.data.fd = udp_sock.get_sockfd();
    ev.events = EPOLLIN | EPOLLET;
    int ret = epoll_ctl(
        reactor->work_epfd
        , EPOLL_CTL_ADD
        , udp_sock.get_sockfd()
        , &ev);
	add_fd(reactor, udp_sock.get_sockfg());
	return 0;


}
*/
int reactor_add_listener(
		reactor_t* reactor
		, const char* ip
		, uint16_t port);

int accept_callback(int fd, void* args);

// echo callbacks
int recv_callback(int fd, void* args);
int send_callback(int fd, void* args);


// rpcapi server callback
int rpcapi_recv_callback(int fd, void* args);




