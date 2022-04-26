
#include <unistd.h>
#include <time.h>
//#include <chrono.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sstream>
#include <fcntl.h>
#include <stdint.h>

#include "util.h"
#include "Log.h"
#include "Socket.h"
/*
std::string sp_file(const char* file_name) {
	std::vector<size_t> sps;
	size_t nsp = find_char(file_name, strlen(file_name), '\\', sps);
	nsp = nsp > 2 ? 2 : nsp;
	std::string rt(file_name);
	return rt.substr(sps[sps.size() - nsp] + 1);
}
*/
#define ERR_STR std::string("[ERROR]  ")         \
    + sp_file(__FILE__) + ":"                   \
    + std::to_string(__LINE__) + "  "            \
    + __FUNCTION__ + " > "                      \
    + strerror(errno)                           \
    + "\n"                                      \
//

namespace tt {
namespace system {

void set_fd_mode(socket_desc_t sock, uint32_t toBlock) {
    int flags = fcntl(sock, F_GETFL, 0);  
    if(toBlock) {
//        TT_DEBUG << "set_fd_mode to block" << "sock = " << sock;
        fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);    //设置成阻塞模式
    } else {
        TT_DEBUG << "set_fd_mode to noblock" << "sock = " << sock;
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；
    }
    
}

//地址复用
void set_reuseaddr(int sockfd, int optval)
{
    int on = (optval != 0) ? 1 : 0;
    //int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
    /*　sockfd：标识一个套接口的描述字。
　　    level：选项定义的层次；支持SOL_SOCKET、IPPROTO_TCP、IPPROTO_IP和IPPROTO_IPV6。
　　    optname：需设置的选项。
　　    optval：指针，指向存放选项值的缓冲区。
　　    optlen：optval缓冲区长度。
        返回值：  成功返回0，失败返回 -1.  */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        return;
}

//端口复用
void set_reuseport(int sockfd, int optval)
{
#ifdef SO_REUSEPORT
    int on = (optval != 0) ? 1 : 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) < 0)
        return;
#else
    fprintf(stderr, "SO_REUSEPORT is not supported.\n");
#endif //SO_REUSEPORT
}



void IPv4Address::to_string(std::string& str) const {
    str = "";
    std::stringstream ss;
    ss << "[IPv4Address]" << "\n"
        << "ip   = \"" << m_ip << "\"\n"
        << "port = " << std::to_string(m_port) << "\n";
    str = ss.str();
}
void IPv4Address::set_addr(const void* addr)
{ 
    m_addr = *(sockaddr_in*)addr; 
    m_ip = inet_ntoa(m_addr.sin_addr);
    m_port = htons(m_addr.sin_port);
}
void* IPv4Address::get_addr_st() { 
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(m_port);
    m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());

 //   TT_DEBUG << "port:" << m_addr.sin_port
 //       << "ip : " << m_addr.sin_addr.s_addr;
    return &m_addr; 
}

bool PosixSocketApi::close() { 
    bool rt = true;
    if(m_sock != 0)
        rt = ::close(m_sock) == 0;
    m_sock = 0;
    return rt;
}

bool PosixSocketApi::to_string(std::string& str) {
    str = "";
    std::stringstream ss;
    ss << "[PosixSocketApi]\n"
        << "m_sock = " << m_sock;
    str = ss.str();
    return true;
}

bool PosixSocketApi::bind(Address::ptr addr)  {
    bool rt = true;
    
    if(addr == nullptr) {
        rt = ::bind(m_sock, NULL, 0);
    } else {
        IPAddress* ip_addr = (IPAddress*)addr.get();
        sockaddr* sock_in = (sockaddr*)ip_addr->get_addr_st();
        rt = ::bind(m_sock, sock_in, sizeof(*sock_in)) != SOCKET_ERROR;
    }
    
 
    return rt;
}

bool PosixSocketApi::listen(uint64_t n_sock) {
    bool rt = true;
    rt = ::listen(m_sock, n_sock) != SOCKET_ERROR;

    return rt;
}
    
bool PosixSocketApi::connect(Address::ptr addr) {
    bool rt = true;
    IPAddress* ip_addr = (IPAddress*)addr.get();
    sockaddr* sock_in = (sockaddr*)ip_addr->get_addr_st();
    rt = ::connect(m_sock, sock_in, sizeof(*sock_in)) != SOCKET_ERROR;
	
    return rt;
}
bool PosixSocketApi::accept(socket_desc_t& ans, uint32_t toBlock) {
    bool rt = true;
    sockaddr addr;
    socklen_t len;

    socket_desc_t client = ::accept(m_sock, &addr, &len);
    set_fd_mode(client, toBlock);
    ans = client;
    rt = ans != INVALID_SOCKET;
    TT_DEBUG << "client sock = " << client;
    return rt;
}
bool PosixSocketApi::send(const void* buffer, size_t& n_send) {
    bool rt = true;
    n_send = ::send(m_sock, buffer, n_send, 0);
    rt = n_send != SOCKET_ERROR;
    return rt;
}
bool PosixSocketApi::recv(void* buffer, size_t& n_recv) {
    bool rt = true;
    n_recv = ::recv(m_sock, buffer, n_recv, 0);
    rt = n_recv != SOCKET_ERROR;
    return rt;
}

bool PosixSocketApi::sendto(
    const void* buffer
    , size_t&   n_send
    , void*     addr
    ) 
{
    bool rt = true;
    n_send = ::sendto(m_sock
        , buffer
        , n_send
        , 0
        , (struct sockaddr*)addr
        , sizeof(sockaddr_in)
        );
    rt = n_send != SOCKET_ERROR;
    return rt;
}
bool PosixSocketApi::recvfrom(
    void*       buffer
    , size_t&   n_recv
    , void*     addr) 
{
    bool rt = true;
    socklen_t len;
    n_recv = ::recvfrom(m_sock
        , buffer
        , n_recv
        , 0
        , (struct sockaddr*)addr
        , &len
        );
    rt = n_recv != SOCKET_ERROR;
    return rt;
}

bool TcpSocketApi::socket(uint32_t toBlock) {
    bool rt = true;
    m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if(m_sock == 0) {
        rt = false;
    } else {
        set_fd_mode(m_sock, toBlock);
    }

    int on = 1;
    set_reuseaddr(m_sock, on);
    set_reuseport(m_sock, on);

    return rt;
}

bool UdpSocketApi::socket(uint32_t toBlock) {
    bool rt = true;
    m_sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if(m_sock == 0) {
        rt = false;
    } else {
        set_fd_mode(m_sock, toBlock);
    }
    return rt;
}

void Socket::dump_remote() { 
    if(m_remote_addr) {
        m_remote_addr->dump(); 
    }
}
void Socket::dump_local() {  
    if(m_local_addr) {
        m_local_addr->dump();
    } 
}

Socket::Socket(int fd)
{
    m_posix_api = PosixSocketApi::ptr(new TcpSocketApi);
     if(fd) { set_sockfd(fd); } 
}

bool Socket::init_tcp(const std::string& ip, uint32_t port) {
    bool rt = true;
    // init args;
    IPv4Address *ip_local = new IPv4Address;
    ip_local->set_ip(ip);
    ip_local->set_port(port);
    m_error_handler = ErrHandler::ptr(new ErrHandler(this));
    m_posix_api.reset();
    m_posix_api = PosixSocketApi::ptr(new TcpSocketApi);
    m_local_addr = Address::ptr(ip_local);

    // socket
    rt = m_posix_api->socket();
    if(!rt) {
        m_error_handler->handle_error(ERR_STR);
        return rt;
    }

    // bind
    rt = this->bind(ip, port);
    if(!rt) {
        m_error_handler->handle_error(ERR_STR);
    }
    return rt;
}

bool Socket::init_udp(const std::string& ip, uint32_t port) {
    bool rt = true;
    IPv4Address *ip_local = new IPv4Address;
    ip_local->set_ip(ip);
    ip_local->set_port(port);
    m_error_handler = ErrHandler::ptr(new ErrHandler(this));
    m_posix_api = PosixSocketApi::ptr(new UdpSocketApi);
    m_local_addr = Address::ptr(ip_local);

    // socket
    rt = m_posix_api->socket();
    if(!rt) {
        m_error_handler->handle_error(ERR_STR);
        return rt;
    }
    return rt;
}


bool Socket::close() {
    bool rt = true;
    rt = m_posix_api->close();
    if(!rt) {
        m_error_handler->handle_error(ERR_STR);
    }
    return rt;
}
bool Socket::reconnect(uint64_t nMs) {
    bool rt = true;
    clock_t start = clock();
    IPv4Address* ipv4_addr = (IPv4Address*)m_local_addr.get();

    while(clock() - start < nMs) {
        rt = this->connect(m_remote_addr);
        if(rt) {
            break;
        }
    }

    return rt;
}
bool Socket::bind(const std::string& ip, uint32_t port) {
    bool rt = true;
    IPAddress::ptr local = IPAddress::ptr(new IPv4Address);
    local->set_ip(ip);
    local->set_port(port);
    if(port == 0) {
        m_local_addr = nullptr;
    } else {
        m_local_addr = local;
    }
   
    rt = this->bind(m_local_addr);
    
    return rt;
}
bool Socket::bind(Address::ptr local) {
    bool rt = true;
    rt = m_posix_api->bind(local);
    if(!rt) {
        m_error_handler->handle_error(ERR_STR);
    }else {
        m_local_addr = local;
//        TT_DEBUG << " bind address: ";
        if(m_local_addr == nullptr) {
            IPAddress* addr = new IPv4Address;
            struct sockaddr localaddr;
            socklen_t addrlen;
            getsockname(m_posix_api->m_sock, &localaddr, &addrlen);
            addr->set_addr(&localaddr);
            m_local_addr = Address::ptr(addr);
        }
//        m_local_addr->dump();
    }
    return rt;
}
bool Socket::listen(uint64_t conn_num) {
    bool rt = true;
    rt = m_posix_api->listen(conn_num);
    if(!rt) {
        m_error_handler->handle_error(ERR_STR);
    }
    return rt;
}
bool Socket::accept(Socket& ans) {
    bool rt = true;
    socket_desc_t client_sock = 0;
    rt = m_posix_api->accept(client_sock);
    ans.m_error_handler = ErrHandler::ptr(new ErrHandler(this));
    ans.m_posix_api =  PosixSocketApi::ptr(new TcpSocketApi(client_sock));
    std::string str;
    ans.m_posix_api->to_string(str);
    TT_DEBUG << str;
    if(!rt) {
        m_error_handler->handle_error(ERR_STR);
    }
    return rt;
}
bool Socket::connect(const std::string& ip, uint32_t port) {
    bool rt = true;
    IPAddress::ptr remote = IPAddress::ptr(new IPv4Address);
    remote->set_ip(ip);
    remote->set_port(port);
    m_remote_addr = remote;
    rt = this->connect(m_remote_addr);
    
    return rt;
}
bool Socket::connect(Address::ptr addr) {
    bool rt = true;
    rt = m_posix_api->connect(addr);
    if(!rt) {
        m_error_handler->handle_error(ERR_STR, [this]() {
            this->reconnect();
        }, true);
    } else {
        m_remote_addr = addr;
//        TT_DEBUG << " connect address: ";
//        m_remote_addr->dump();
    }

    return rt;
}
bool Socket::send(const void* buffer, size_t& size) {
    bool rt = true;
    rt = m_posix_api->send(buffer, size);
    if(!rt) {
        m_error_handler->handle_error(ERR_STR, [this, buffer, size]() {
            if(this->reconnect()) {

            }
        }, true);
    }

    return rt;
}
bool Socket::recv(void* buffer, size_t& size) 
{
    bool rt = true;
    rt = m_posix_api->recv(buffer, size);
    if(!rt) {
        TT_DEBUG << "sock = " << this->m_posix_api->m_sock;
        m_error_handler->handle_error(ERR_STR, [this]() {
            this->reconnect();
        }, true);
    }

    return rt;
}
bool Socket::sendto(const void* buffer
    , size_t& size
    , void* addr
    ) 
{
    bool rt = true;
    if(addr == nullptr) 
    {
        addr = m_remote_addr->get_addr_st();
    }
    rt = m_posix_api->sendto(buffer, size, addr);
    if(!rt) 
    {
        m_error_handler->handle_error(ERR_STR);
    }

    return rt;
}
bool Socket::recvfrom(void* buffer
    , size_t& size
    , void* addr
    )
{
    bool rt = true;
    if(addr == nullptr) 
    {
        addr = m_local_addr->get_addr_st();
    }
    rt = m_posix_api->recvfrom(buffer, size, addr);
    if(!rt) 
    {
        m_error_handler->handle_error(ERR_STR);
    }
    return rt;
}


} // namespace system
} // namespace tt
