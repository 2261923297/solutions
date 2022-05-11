#include "reqpool.h"
#include "Socket.h"
#include "Log.h"
#include "util.h"

#include <string.h>
#include <unistd.h>
#include <stdint.h>
static uint32_t n_recv = 0;
bool pt_recv(void* content, size_t& len, void* context) {
    bool rt = true;
#if 0
	n_recv++;
	if(n_recv % 1024 == 0) {
		TT_DEBUG << "recv " << str_val(n_recv);
	}
#endif
//    INFO_SYS << "respond: " 
//			<< std::string((const char*)content, 0, len);
    return rt;
}


void test_base(
		const char* sv_ip
		, uint16_t sv_port
		, size_t n_port
		, bool is_tcp = false) 
{
    std::string say_word = "hello, reqpoll!";
    asynreq_context ac;
    asynreq_context_init(&ac, sv_ip, sv_port);

	const size_t bf_size = 1024 * 4;
	char tmp_buffer[bf_size] = { 0 };
	size_t n_recv = bf_size;

	uint64_t start_time = cur_time_us();
	uint32_t n_commit = 0;
	const uint64_t want_commit = 5 * 1024;

    while(1) {
//		TT_DEBUG << "n_recv: " << n_recv;
		n_recv = bf_size;
#if 0
		uint64_t commit_time = cur_time_us();
		TT_DEBUG << "commit_time_us: " << commit_time;
#endif
		if(is_tcp) {
			asynreq_commit(
				&ac
				, &pt_recv
				, nullptr
				, say_word.c_str()
				, say_word.size()
				, tmp_buffer
				, n_recv);
		} else {
			asynreq_commit_udp(
				&ac
				, &pt_recv
				, nullptr
				, say_word.c_str()
				, say_word.size()
				, tmp_buffer
				, n_recv);
		}
		n_commit ++;
//		break;
		if(n_commit >= want_commit) break;
    }

	TT_DEBUG << str_val(n_commit); 

	uint64_t end_time = cur_time_us();
	uint64_t cost_time = end_time - start_time;
	TT_DEBUG << str_val(cost_time) << "us";

	sleep(3);
    asynreq_context_destory(&ac);
}

int main() 
{
	const char* sv_ip = "192.168.90.1";
#if 0
	uint16_t sv_port = 8800;
	uint16_t sv_udp_port = 9999;
#else
	uint16_t sv_port = 9900;
	uint16_t sv_udp_port = 9998;
#endif

	test_base(sv_ip, sv_udp_port, 100, false);
	test_base(sv_ip, sv_port, 100, true);
	while(1) sleep(1);
    return 0;
}

