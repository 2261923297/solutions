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
    virtual void set_addr(const void* addr) = 0;
    virtual void* get_addr_st() = 0;

    void set_port(uint32_t port) { m_port = port; }
    void set_ip(const std::string& ip) { m_ip = ip; }

    const std::string& get_ip() const { return m_ip; }
    uint32_t get_port() const { return m_port; }
protected:
    std::string m_ip;
    uint32_t    m_port;

};


class IPv4Address : public IPAddress {
public:
    ~IPv4Address() { }
    virtual void to_string(std::string& str) const override;
    virtual void set_addr(const void* addr) override;
    virtual void* get_addr_st() override;
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
    virtual ~PosixSocketApi() { close(); }

    bool close();
    bool to_string(std::string& str);
    virtual bool socket(uint32_t toBlock = 1) = 0;
    bool bind(Address::ptr addr);

    bool listen(uint64_t n_sock);
    bool connect(Address::ptr addr);
    bool accept(socket_desc_t& rt, uint32_t toBlock = 1);
    bool send(const void* buffer, size_t& n_send);
    bool recv(void* buffer, size_t& n_recv);

    bool sendto(const void* buffer
        , size_t& n_send
        , void* addr);
    bool recvfrom(
        void* buffer
        , size_t& n_recv
        , void* addr);
protected:
    socket_desc_t   m_sock;
}; // PosixSocketApi

// 具体实现类
class TcpSocketApi : public PosixSocketApi {
public:
    using ptr = std::shared_ptr<TcpSocketApi>;
    TcpSocketApi(socket_desc_t sock = 0) 
        : PosixSocketApi() { m_sock = sock; }

    virtual bool socket(uint32_t toBlock = 1) override;
    
}; // class TcpSocket


class UdpSocketApi : public PosixSocketApi {
public:
    using ptr = std::shared_ptr<TcpSocketApi>;
    UdpSocketApi(socket_desc_t sock = 0) { m_sock = sock; }
    ~UdpSocketApi() { close(); }

    virtual bool socket(uint32_t toBlock = 1) override;

}; // class UdpSocketApi

class Socket {
public:
    using ptr = std::shared_ptr<Socket>;
    Socket() { }
    virtual ~Socket() { }

    virtual bool init_tcp(
        const std::string& ip = "0.0.0.0"
        , uint32_t port = 7788
        );
    virtual bool init_udp(
        const std::string& ip = "0.0.0.0"
        , uint32_t port = 7788
        );

    socket_desc_t get_sockfd() const 
        { return m_posix_api->m_sock; }
    // 重连等待时间 60s
    bool reconnect(uint64_t nMs = 60000);
    bool close();
    bool bind(const std::string& ip = "", uint32_t port = 0);
    bool bind(Address::ptr local);
    bool listen(uint64_t conn_num = SOMAXCONN);
    bool accept(Socket& ans);
    bool connect(const std::string& ip, uint32_t port);
    bool connect(Address::ptr remote);
    bool send(const void* buffer, size_t& size);
    bool recv(void* buffer, size_t& n_recv);
    bool sendto(const void* buffer
        , size_t& n_send
        , void* addr = nullptr);
    bool recvfrom(void* buffer
        , size_t& n_recv
        , void* addr = nullptr);
    
    void dump_remote();
    void dump_local();
protected:
    PosixSocketApi::ptr m_posix_api;
    ErrHandler::ptr m_error_handler;
    Address::ptr m_remote_addr;
    Address::ptr m_local_addr;

};  // class Socket

} // namespace system
} // namespace tt



