#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "../../include/Log.h"
#include "../../include/File.h"

namespace tt {
namespace net {

class Socket { 
public: 
	using ptr = std::shared_ptr<Socket>;
	using weak_ptr = std::shared_ptr<Socket> weak_ptr;

	Socket();
	Socket(int family, int type, int protocol = 0);
	~Socket();
	enum Type {
		TCP = SOCK_STREAM,
		UDP = SOCK_DGRAM
	};

	enum Family {
		IPv4 = AF_INET,
		IPv6 = AF_INET6,
		UNIX = AF_UNIX
	};

protected:
	void initSocke();
	void newSocke();
	virtual bool init(int sock);
protected:
	int m_sock;
	int m_family;
	int m_type;
	int m_protocol;
	bool m_isConnected;

	Address::ptr m_localAddress;
	Address::ptr m_remoteAddress;

}; // class Socket

class SSLSocker : public Socket {
public:
	using ptr = std::shared_ptr<SSLSocket>;


}; // class SSLSocket

} // namespace net
} // namespace tt


