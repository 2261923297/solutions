#include "TNet.h" 

using namespace tt::net;

void
test_base() { 
	Address::ptr ser_addr(IPv4Address::Create("127.0.0.1", 8088));
	Socket s(AF_INET, SOCK_STREAM);
	s.bind(ser_addr);
	TT_DEBUG << s.to_string();
	s.listen();
	system("netstat -anp ");
}

int
main(int argc, char** argv) {
	DEBUG_SYS << "Hello, TNet!";
	test_base();

	return 0; 
}





