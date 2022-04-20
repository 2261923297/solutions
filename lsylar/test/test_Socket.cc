#include "Socket.h"
#include "Log.h"
#include "ErrHandler.h"
#include "util.h"
#include <string.h>
#include <thread>
#include <chrono>
#include <time.h>
#include <stdint.h>

using namespace tt::system;
#define IP "192.168.43.110"

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
    conn_client->init_tcp(IP, 7890);
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
        udp_echo_server.recvfrom(echo_buffer, n_recv, &saddr);
        TT_DEBUG << "echo recv: " 
            << std::string(echo_buffer, n_recv);
        udp_echo_server.sendto(echo_buffer, n_recv, &saddr);
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
                << "k ... time_cost " << end - start << "ms";
            start = end;
        }
    }
}
int main(int argc, char** argv) {
    if(argc < 2) {
//        TT_DEBUG << "EchoServer: ";
//        test_base();
//        test_udp_echo_server();
        test_conn_till();
    } else {
        TT_DEBUG << "SendClient: ";
        test_client();
    }
	
	return 0;
}
