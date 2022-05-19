#pragma once

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


/**
 *  asynchronous_request_pool only get request by socket, not forward 
 *
 * */

#define IP "192.168.43.110"
typedef bool (*asynreq_result_cb)(void* content, size_t& len, void* cb_context);
static uint64_t __time = 0;

/*
    shared_ptr 对象的申请不能用malloc
    shared_ptr 引用会出问题, 
        sock = 5 引用会出现sock = 1, 去掉引用就没问题了
*/
struct eparg_t {
    // 负责发送请求的 socket
    tt::system::Socket::ptr req_sender;
    // 作为 client 处理一下从 server 接收到的结果
    asynreq_result_cb cb;
	void* cb_context;
	void* result_buffer;
	size_t& result_buffer_len;

    eparg_t(
			tt::system::Socket::ptr& _req_sender
			, asynreq_result_cb _cb 
			, void* _cb_context
			, void* _result_buffer
			, size_t& _result_buffer_len
		   )
        : req_sender(_req_sender)
        , cb(_cb) 
		, cb_context(_cb_context)
		, result_buffer(_result_buffer)
		, result_buffer_len(_result_buffer_len) 
    { }
  
};


struct asynreq_context {
    int epfd;
    pthread_t thid;

	// send 
	char server_ip[16];
	uint16_t server_port;
};

// epoll wait & cbarg->result_sender->send(tmp_buffer)
void* asynreq_callback(void* cbarg);

// func create req_sock & req_sock->send(send_buffer, send_len);
bool asynreq_commit(
			asynreq_context* asyncreq_ctx
			, asynreq_result_cb _cb
			, void* _cb_context
            , const void* request_send_buffer
            , size_t request_send_len
			, void* result_buffer
			, size_t& result_buffer_len);

// func create req_sock & req_sock->send(send_buffer, send_len);
bool asynreq_commit_udp(
			asynreq_context* asyncreq_ctx
			, asynreq_result_cb _cb
			, void* _cb_context
            , const void* request_send_buffer
            , size_t request_send_len
			, void* result_buffer
			, size_t& result_buffer_len);

// sv_ip: get req from server which ip == sv_ip
//
bool asynreq_context_init(asynreq_context* asynreq_ctx
		, const char* sv_ip = IP
		, uint16_t sv_port = 7788
	);
bool asynreq_context_destory(asynreq_context* asynreq_ctx);



// listenfd 用水平触发


