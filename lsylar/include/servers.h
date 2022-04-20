#include <sys/epoll.h>
#include <string>
#include <stdint.h>
#include <vector>

static const int SR_BUFFER_LEN = 512;
static const int FD_ITEM_BLOCK_SIZE = 1024;

static const int EPOLL_WORK_EVENT_SIZE = 1024;
static const int EPOLL_LISTEN_EVENT_SIZE = 1024;

typedef int (*NET_EVENT_CB)(int fd, void* args);

struct __st_reactor;


typedef struct __st_fd_item {
    int fd;
    int event;

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

// 自己创建 reactor 管理fd_item原因: 减少对 epoll 的依赖
typedef struct __st_reactor {
    int listen_epfd;
    int work_epfd;

    size_t nfd;
    std::vector<fd_item_block_t*> fd_item_blocks;
    
} reactor_t;

int add_fd(reactor_t* reactor, int fd);
int del_fd(reactor_t* reactor, int fd);
fd_item_t* get_fd_item(reactor_t* reactor, int fd);

int init_reactor(reactor_t *reactor);
int destory_reactor(reactor_t* reactor);

// recv and send
int work_loop(reactor_t* reactor);
// only listen
int listen_loop(reactor_t* reactor);


// echo callback
int recv_callback(int fd, void* args);
int send_callback(int fd, void* args);
int accept_callback(int fd, void* args);


