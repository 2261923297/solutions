#include "reqpool.h"
#include "Socket.h"
#include "Log.h"
#include "util.h"

#include <string.h>
#include <unistd.h>
#include <stdint.h>

bool pt_recv(void* content, size_t& len, void* context) {
    bool rt = true;
//    INFO_SYS << "respond: " 
//			<< std::string((const char*)content, 0, len);
    return rt;
}


void test_base(
		const char* sv_ip
		, uint16_t sv_port
		, size_t n_port) 
{
    std::string say_word = "hello, reqpoll!";
    asynreq_context ac;
    asynreq_context_init(&ac, sv_ip, sv_port);

	const size_t bf_size = 1024 * 4;
	char tmp_buffer[bf_size] = { 0 };
	size_t n_recv = bf_size;

	uint64_t start_time = cur_time_us();
	uint32_t n_commit = 0;
	const uint64_t want_commit = 20 * 1000;
    while(1) {
//		TT_DEBUG << "n_recv: " << n_recv;
		n_recv = bf_size;

//		TT_DEBUG << "commit_time_us: " << commit_time;
        asynreq_commit(
			&ac
			, &pt_recv
			, nullptr
			, say_word.c_str()
			, say_word.size()
			, tmp_buffer
			, n_recv);
		n_commit ++;
		if(n_commit >= want_commit) break;
    }
	TT_DEBUG << str_val(n_commit); 

	uint64_t commit_time = cur_time_us();
	uint64_t cost_time = commit_time - start_time;
	TT_DEBUG << str_val(cost_time);

	while(1) 
	{
		sleep(1);
	}
    asynreq_context_destory(&ac);
}


int main() 
{
	const char* sv_ip = "192.168.90.1";
	uint16_t sv_port = 8888;
	
	test_base(sv_ip, sv_port, 100);
    return 0;
}
