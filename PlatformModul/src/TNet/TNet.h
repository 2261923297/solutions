#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "Address.h"
#include "Log.h"
#include "File.h"

namespace tt {
namespace net {

class Socket { 
public: 
	using ptr = std::shared_ptr<Socket>;
	using weak_ptr = std::weak_ptr<Socket>;

	static Socket::ptr CreateTCP(tt::net::Address::ptr addr);
	static Socket::ptr CreateUDP(tt::net::Address::ptr addr);
	static Socket::ptr CreateTCP6();
	static Socket::ptr CreateUDP6();
	enum Type {
		TCP = SOCK_STREAM,
		UDP = SOCK_DGRAM
	};

	enum Family {
		IPv4 = AF_INET,
		IPv6 = AF_INET6,
		UNIX = AF_UNIX
	};

	Socket();
	virtual ~Socket();

	Socket(int family, int type, int protocol = 0);

	virtual Socket::ptr accept();
	virtual bool bind(const Address::ptr addr);
	virtual bool connect(const Address::ptr addr);
	virtual bool reconnect();
	virtual bool listen(int backlog = SOMAXCONN);
	virtual bool close();

	virtual int send(const void* buffer, size_t len, int flags = 0);
	virtual int send_to(const void* buffer, size_t len, const Address::ptr to, int flags = 0);
	
	virtual int recv(void* buffer, size_t len, int flags = 0);
	virtual int recv_from(void* buffer, size_t len, const Address::ptr to, int flags = 0);

	virtual std::ostream& dump(std::ostream& os) const;
	virtual std::string to_string() const;

	Address::ptr remote_addr();
	Address::ptr local_addr();
	
	int family() const { return m_family; }
	int type() const { return m_type; }
	int protocol()const { return m_protocol; }
	int sock() const { return m_sock; }

	bool is_connected() const { return m_is_connected;}
	bool is_valid() const { return m_sock != -1; }

	bool cancel_read() { return 0; }
	bool cancel_write() { return 0; }
	bool cancel_accept() { return 0; }
	bool cancel_all() { return 0; }

	int get_error();


protected:

	void init_sock();
	void new_sock();
	virtual bool init(int sock);

protected:
	int m_sock;
	int m_family;
	int m_type;
	int m_protocol;
	bool m_is_connected;

	Address::ptr m_local_addr;
	Address::ptr m_remote_addr;

}; // class Socket

class SSLSocket : public Socket {
public:
	using ptr = std::shared_ptr<SSLSocket>;


}; // class SSLSocket

} // namespace net
} // namespace tt


