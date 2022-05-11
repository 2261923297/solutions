#pragma once

#include <sys/socket.h>
#include <memory>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include "ErrHandler.h"
namespace tt {
namespace system {

#ifndef socket_desc_t 
#define socket_desc_t int
#endif // socket_desc_t

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (~0)

class Socket;
class Address {
public:
    using ptr = std::shared_ptr<Address>;
    virtual ~Address() { }

    virtual void to_string(std::string& str) const = 0;
    virtual void set_addr(const void* addr) = 0;
    virtual void* get_addr_st() = 0;

	virtual void update_addr() = 0;

    virtual void dump() { 
        std::string str;
        to_string(str);
        std::cout << str << std::endl;
    }

private:

};

class IPAddress : public Address {
public:
    using ptr = std::shared_ptr<IPAddress>;
    virtual ~IPAddress() { }

    virtual void to_string(std::string& str) const = 0;
	// copy init m_ip m_port
    virtual void set_addr(const void* addr) = 0;
	// init m_addr from m_ip m_port
    virtual void* get_addr_st() = 0;

    virtual void set_port(uint32_t port) = 0; 
    virtual void set_ip(const std::string& ip) = 0;

    const std::string& get_ip() const { return m_ip; }
    uint32_t get_port() const { return m_port; }

	virtual void update_addr() = 0;

protected:
    std::string m_ip;
    uint32_t    m_port;

};


class IPv4Address : public IPAddress {
public:
	IPv4Address();
    ~IPv4Address() { }
    virtual void to_string(std::string& str) const override;

    virtual void set_addr(const void* addr) override;
    virtual void* get_addr_st() override;

    virtual void set_port(uint32_t port) override; 
    virtual void set_ip(const std::string& ip) override;

	virtual void update_addr() override;
private:
    sockaddr_in m_addr;
};

class Socket;
// interface
class PosixSocketApi {
public:
    friend Socket;
    using ptr = std::shared_ptr<PosixSocketApi>;
    PosixSocketApi() { }
    virtual ~PosixSocketApi() {  }
    bool to_string(std::string& str);

    bool close();
    virtual bool socket(uint32_t toBlock = 1) = 0;

    virtual bool send(const void* buffer, size_t& n_send) = 0;
    virtual bool recv(void* buffer, size_t& n_recv) = 0;

    bool bind(Address::ptr addr);
    bool listen(uint64_t n_sock);
    bool connect(Address::ptr addr);
    bool accept(socket_desc_t& rt, uint32_t toBlock = 1);

protected:
    socket_desc_t   m_sock;
}; // PosixSocketApi

// 具体实现类
class TcpSocketApi : public PosixSocketApi {
public:
    using ptr = std::shared_ptr<TcpSocketApi>;
    TcpSocketApi(socket_desc_t sock = 0) 
			{ m_sock = sock; }
    virtual bool socket(uint32_t toBlock = 1) override;

    bool send(const void* buffer, size_t& n_send) override;
    bool recv(void* buffer, size_t& n_recv) override;

}; // class TcpSocket


class UdpSocketApi : public PosixSocketApi {
public:
    using ptr = std::shared_ptr<TcpSocketApi>;
    UdpSocketApi(Socket* caller 
			, socket_desc_t sock = 0) 
		  : m_caller(caller)
			{ m_sock = sock; }

    ~UdpSocketApi() { close(); }

    virtual bool socket(uint32_t toBlock = 1) override;

    bool send(const void* buffer, size_t& n_send) override;
    bool recv(void* buffer, size_t& n_recv) override;
private:
	Socket*			m_caller;
}; // class UdpSocketApi

class Socket 
{
public:
    using ptr = std::shared_ptr<Socket>;
	friend UdpSocketApi;

    Socket(int fd = 0, bool is_tcp = true);
    virtual ~Socket() { close(); }

    virtual bool init_tcp(
        const std::string& ip = "0.0.0.0"
        , uint32_t port = 7788
    );
    virtual bool init_udp(
        const std::string& ip = "0.0.0.0"
        , uint32_t port = 7788
    );
    bool close();

	void set_local_addr(Address::ptr local) { m_local_addr = local; }
	void set_remote_addr(Address::ptr remote) { m_remote_addr = remote; }

    socket_desc_t get_sockfd() const { return m_posix_api->m_sock; }
    void set_sockfd(socket_desc_t sock) { m_posix_api->m_sock = sock; }

    void dump_remote();
    void dump_local();

    // 重连等待时间 6s
	// posix api
    bool reconnect(uint64_t nMs = 6000);
	bool socket();
    bool bind(const std::string& ip = "", uint32_t port = 0);
    bool bind(Address::ptr local);
    bool listen(uint64_t conn_num = SOMAXCONN);
    bool accept(Socket& ans);
    bool connect(const std::string& ip, uint32_t port);
    bool connect(Address::ptr remote);

    bool send(const void* buffer
			, size_t& size
			, Address::ptr addr = nullptr);
    bool recv(void* buffer
			, size_t& n_recv
			, Address::ptr addr = nullptr);

protected:
    PosixSocketApi::ptr				m_posix_api;
    ErrHandler::ptr					m_error_handler;
    Address::ptr					m_remote_addr;
    Address::ptr					m_local_addr;
};  // class Socket


} // namespace system
} // namespace tt



