#include <sys/socket.h>

int renum(int num) {
	int tmp = num;
	int rt = 0;
	int mod = i;
	std::list<int> num_stack;
	while(tmp) {
		num_stack.push_back(tmp % 10);
		tmp /= 10;
	}
	int i = 0;
	for(auto it = num_stack.begin(); it != num_stack.end(); it++) {
		
	}
	return rt;
}

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in sin;
	sin
	bind(sock, )
	return 0;
}
