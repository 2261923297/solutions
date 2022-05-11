#include "Socket.h"
#include "Log.h"
#include "ErrHandler.h"
#include "util.h"


#include <string.h>
#include <thread>
#include <chrono>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

using namespace tt::system;
#define IP "192.168.90.1"

void test_base() {
    Socket::ptr echo_server(new Socket);
    Socket conn_client;
    echo_server->init_tcp(IP); 
    echo_server->listen(1);
    echo_server->accept(conn_client);

    const size_t buffer_size = 1500;
    char echo_buffer[buffer_size] = { 0 };
    size_t n_recv = 1500;
    size_t n_send = 1500;
    bool rt = true;
    
    while(rt) {
        n_recv = 1500;
        rt = conn_client.recv(echo_buffer, n_recv);
        if(!rt) {
            TT_DEBUG << "echo_server quit";
            break;
        }
        if(n_recv > 0) {
			/*
            TT_DEBUG << "echo_server recv: " 
                << std::string(echo_buffer, n_recv);
			*/
            rt = conn_client.send(echo_buffer, n_recv);
            if(!rt) {
                TT_DEBUG << "echo_server quit";
                break;
            }
        }
//        sleep(100);
    }
}
void test_client() {
    Socket::ptr conn_client(new Socket);
    conn_client->init_tcp(IP, 0);
    conn_client->connect(IP, 7788);
    char buffer[128] = { 0 };
    std::string say_world = "hello, tcp server!";
    size_t n_recv = 0, n_send = 0;
    
    print_val(say_world.size(), ld);
	const uint64_t target_send = 100 * 1000;
	uint64_t n_commit = 0;
    while(1) {
		memset(buffer, 0, n_recv);
        n_recv = 1500;
        n_send = say_world.size();
        conn_client->send(say_world.c_str(), n_send);
        conn_client->recv(buffer, n_recv);
		n_commit++;
		if(n_commit >= target_send)
		{
			break;
		}
//        TT_DEBUG << "what recv: " << std::string(buffer, n_recv);
    }
}

void test_conn_till() {
    int n_conn = 0;
    uint64_t start = cur_time_ms();
    std::vector<tt::system::Socket*> socks;
	uint32_t nk = 0;
    while(1) {
        tt::system::Socket* client = new(tt::system::Socket);
        client->init_tcp("0.0.0.0", 0);
        client->connect("192.168.90.100", 7788);
        socks.push_back(client);
		
        n_conn++;

        uint64_t end = cur_time_ms();
        if(n_conn % 1024 == 0) {
			nk++;
            TT_DEBUG << "connected " << n_conn 
                << " ... time_cost " << end - start << "ms";
            start = end;
			if(nk >= 20) { break; }
        }
        if(n_conn == 20480) {
                break; 
        }
    }
    while(1) {
        sleep(1);
    }
}

int get_sock_buffer_size() {
    int defRcvBufSize = -1;
     socklen_t optlen = sizeof(defRcvBufSize);
     tt::system::Socket s;
     s.init_udp();
     if (getsockopt(s.get_sockfd(), SOL_SOCKET, SO_RCVBUF, &defRcvBufSize, &optlen) < 0)
     {
         printf("getsockopt error=%d(%s)!!!\n", errno, strerror(errno));
         return 0;
     }
     return defRcvBufSize;
}

void test_udp_server()
{
	tt::system::Socket::ptr u_server(
			new tt::system::Socket);
	u_server->init_udp("192.168.90.1", 8888);

	const int bf_size = 1 * 1024 - 1;
	char* buffer[bf_size + 1] = { 0 };
	size_t n_recv = bf_size;
	
	IPAddress::ptr client_address(new IPv4Address);
	while(1) {
		memset(buffer, 0, n_recv);
		n_recv = bf_size;
		u_server->recv(
				buffer
				, n_recv
				, client_address);
		u_server->send(
				buffer
				, n_recv
		);
	}
}

void test_udp_client()
{
	tt::system::Socket::ptr u_server(
			new tt::system::Socket);
	u_server->init_udp("192.168.90.1"
			, (uint16_t)cur_time_us());

	IPAddress::ptr server_address(new IPv4Address);
	server_address->set_ip("192.168.90.1");
	server_address->set_port(8888);

	const char		*say_word = "hello, udp server!";
	size_t			sw_len = strlen(say_word);
	const int		bf_size = 1 * 1024 - 1;
	char			*buffer[bf_size + 1] = { 0 };
	size_t			n_recv = bf_size;

	int n_cycle = 0;
	const size_t target_cyc = 100 * 1000; 

	while(1)
	{
		size_t n_send = sw_len;
		n_recv = bf_size;
		u_server->send(
				say_word
				, n_send
				, server_address);
		u_server->recv(buffer
				, n_recv
				, server_address);
//		TT_DEBUG << "recv: " << std::string((char*)buffer, n_recv);
		n_cycle++;
		if(n_cycle == target_cyc)
		{
			break;
		}
    }
}

void test_server(
		tt::system::Socket::ptr server
		, const uint64_t cache_len)
{
	size_t n_send = 0;
	size_t n_recv = 0;
	char buffer[cache_len + 1] = { 0 };
	memset(buffer, 'T', cache_len);
	for(;;)
	{
		n_recv = cache_len;
		server->recv(buffer, n_recv);
		server->send(buffer, n_recv);
	}
}

void test_client(
		tt::system::Socket::ptr client
		, const uint64_t buffer_len
		, const uint64_t target_cycle)
{
	size_t n_send = 0;
	size_t n_recv = 0;
	char buffer[buffer_len + 1] = { 0 };
	memset(buffer, 'T', buffer_len);
	for(uint64_t n_cycle = 0; n_cycle < target_cycle; n_cycle++)
	{
		n_send = buffer_len;
		n_recv = buffer_len;
		client->send(buffer, n_send);
		client->recv(buffer, n_recv);
	}
}

static const size_t 
	s_cycle = 10 * 1000;
using namespace tt::system;
void test_ut_speed_client(bool is_udp = 0)
{
	Socket::ptr sock(new Socket);

	const char* server_ip = "192.168.90.1";
	const uint16_t server_port = 8888;
	if(is_udp)
		sock->init_udp(server_ip
			, (uint16_t)cur_time_us());
	else 
		sock->init_tcp(server_ip
			, (uint16_t)cur_time_us());
	sock->connect(server_ip, server_port);
	test_client(sock, 1400, s_cycle);
}	

void test_ut_speed_server(bool is_udp = 0)
{
	Socket::ptr sock(new Socket)
				, client(new Socket);

	const char* server_ip = "192.168.90.1";
	const uint16_t server_port = 8888;
	if(is_udp) {
		IPAddress::ptr addr(new IPv4Address);
		sock->init_udp(server_ip, server_port);
		sock->set_remote_addr(addr);
		test_server(sock, 1400);
	}
	else
	{
		sock->init_tcp(server_ip, server_port);
		sock->listen(1);
		sock->accept(*client);
		test_server(client, 1400);
	}
	TT_DEBUG << client->get_sockfd();
}
#define TEST_UDP 0

int main(int argc, char** argv) {
	bool is_udp = true;
    if(argc < 2) {
        TT_DEBUG << "EchoServer: ";
//        test_client();
//        test_udp_echo_server();
        test_conn_till();
//        TT_DEBUG << get_sock_buffer_size();
//		test_ut_speed_server(is_udp);
    } else {
		test_func(test_ut_speed_client, is_udp);
    }
	
	return 0;
}
