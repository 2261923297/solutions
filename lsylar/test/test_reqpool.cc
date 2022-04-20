#include "reqpool.h"
#include <unistd.h>
#include "Socket.h"

#define IP "192.168.43.110"

void test_base() 
{
    tt::system::Socket::ptr 
            client_sock(new tt::system::Socket);
    client_sock->init_tcp(IP, 7890);
    client_sock->connect(IP, 7788);

    asynreq_context ac;
    asynreq_context_init(&ac);
    asynreq_commit(&ac, client_sock);

    
    while(1) {
        sleep(1);

    }
//    asynreq_context_destory(&ac);
}


int main() {
    test_base();

    return 0;
}