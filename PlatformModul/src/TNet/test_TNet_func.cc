#include "TNet.h" 
#include <thread>
#include <unistd.h>

using namespace tt::net;
Socket::ptr 
init_server_sock(const char* ip, uint16_t port) {
	Address::ptr ser_addr(IPv4Address::Create("127.0.0.1", 8088));
	Socket::ptr s(new Socket(AF_INET, SOCK_STREAM));
	s->bind(ser_addr);
	s->listen();

	return s;
}


void single_echo_server() {
	Socket::ptr echo_server = init_server_sock("127.0.0.1", 8088);
	if(echo_server.get() != nullptr) 
		TT_DEBUG << "init_server_sock: " << echo_server->to_string();
	else 
		return;
	
	Socket::ptr client_sock;

	char buffer[4096] = { 0 };
	int buffer_size = 4096;
	int re_accept = 1;
	while(1) {
		while(re_accept) {
			client_sock = echo_server->accept();
			if(nullptr != client_sock) {
				re_accept = 0;
				break;
			}
			TT_DEBUG << "wait_client ...";
		}

		if(client_sock->recv(buffer, buffer_size) <= 0) {
			TT_DEBUG << "client: " << client_sock->to_string() << "is out"; 
			client_sock->close();
			re_accept = 1;
		} else {
			TT_DEBUG << "\nrecv:" << client_sock->to_string() << ": " << buffer; 
		}
	}
}


void only_send_client() {
	Address::ptr ser_addr(IPv4Address::Create("127.0.0.1", 8090));
	Socket::ptr client(new Socket(AF_INET, SOCK_STREAM));
	if(!client->connect(ser_addr)) {
		TT_DEBUG << "Can^t connect";
		return;
	}
	TT_DEBUG << "client contected\nlocal_addr" << client->local_addr()->to_string()
			<< "\nremote_addr: " << client->local_addr()->to_string();
	
	const std::string msg = "Hello, echo_server";
	TT_DEBUG << "begin send";
	
	while(client->send(msg.c_str(), msg.size())) {
		std::cout << "send 1\n";
	//	break;
	}
}

void
test_base() { 
}

int
main(int argc, char** argv) {
//	std::thread trd_echo_server(single_echo_server);
//	std::thread trd_only_send_clent(only_send_client);
	only_send_client();
/*
	trd_echo_server.detach();

	for(int i = 0; i < 50000; i++) {
		for(int j = 0; j < 100; j++) {

		}
	}

	
	trd_only_send_clent.detach();
*/
	while(1) {
		sleep(1000);
	}
	return 0; 
}

