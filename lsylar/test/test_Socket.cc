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
#define IP "192.168.90.100"

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
            TT_DEBUG << "echo_server recv: " 
                << std::string(echo_buffer, n_recv);
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
    std::string say_world = "Hello, profect Socket!";
    size_t n_recv = 0, n_send = 0;
    
    print_val(say_world.size(), ld);
    while(1) {
        n_recv = 1500;
        n_send = say_world.size();
        conn_client->send(say_world.c_str(), n_send);
        conn_client->recv(buffer, n_recv);
        TT_DEBUG << "what recv: " << std::string(buffer, n_recv);
        sleep(1);
    }
}

void test_udp_echo_server() {
    Socket udp_echo_server;
    udp_echo_server.init_udp(IP);  

    const size_t buffer_size = 1500;
    char echo_buffer[buffer_size] = { 0 };
    size_t n_recv = 1500;
    size_t n_send = 1500; 

    sockaddr saddr;
    while(1) 
    {
        n_recv = buffer_size;
        udp_echo_server.recvfrom(echo_buffer, n_recv, nullptr);
        TT_DEBUG << "echo recv: " 
            << std::string(echo_buffer, n_recv);
        udp_echo_server.sendto(echo_buffer, n_recv, nullptr);
    }
}

void test_udp_send_client() {
    
}


void test_conn_till() {
    int n_conn = 0;
    uint64_t start = cur_time_ms();
    std::vector<tt::system::Socket*> socks;
    while(1) {
        tt::system::Socket* client = new(tt::system::Socket);
        client->init_tcp("0.0.0.0", 0);
        client->connect(IP, 7788);
        socks.push_back(client);

        n_conn++;
        uint64_t end = cur_time_ms();
        if(n_conn % 1024 == 0) {
            TT_DEBUG << "connected " << n_conn 
                << " ... time_cost " << end - start << "ms";
            start = end;
            
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
		u_server->recvfrom(
				buffer
				, n_recv
				, client_address);
/*		
		TT_DEBUG << "recv_from: "
//			<< client_address->get_ip()  << ":" 
//			<< client_address->get_port()
			<< " n_recv = " << n_recv 
			<< " > " << std::string((char*)buffer, n_recv);
*/
		u_server->dump_remote(); // 几乎没有延时 4ms左右
		u_server->sendto(
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

	const char* say_word = "hello udp server!";
	size_t sw_len = strlen(say_word);
	const int bf_size = 1 * 1024 - 1;
	char* buffer[bf_size + 1] = { 0 };
	size_t n_recv = bf_size;

	int n_cycle = 0;
	const size_t target_cyc = 50 * 1000; 
	while(1)
	{
		size_t n_send = sw_len;
		n_recv = bf_size;
		u_server->sendto(
				say_word
				, n_send
				, server_address);
		u_server->recvfrom(buffer
				, n_recv
				, server_address);
//		TT_DEBUG << "recvfrom: " << std::string((char*)buffer, n_recv);
		n_cycle++;
		if(n_cycle == target_cyc)
		{
			break;
		}
    }

}

int main(int argc, char** argv) {
    if(argc < 2) {
        TT_DEBUG << "EchoServer: ";
//        test_base();
//        test_client();
//        test_udp_echo_server();
//        test_conn_till();
//        TT_DEBUG << get_sock_buffer_size();
		test_udp_server();        
    } else {
        TT_DEBUG << "SendClient: ";
        test_func(test_udp_client);
    }
	
	return 0;
}
