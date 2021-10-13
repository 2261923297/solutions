#include "TNet.h" 
#include <limits.h>

namespace tt {
namespace net {

Socket::Socket(int family, int type, int protocol)
	: m_sock(-1)
	, m_family(family) 
	, m_type(type)
	, m_protocol(protocol)
	, m_is_connected(false) { 

}

Socket::~Socket() {
	close();
}

void Socket::init_sock() {
	int val = 1;
}
void Socket::new_sock() {
	m_sock = socket(m_family, m_type, m_protocol);
	if(m_sock != -1) {
		close();
	}
	init_sock();

}
bool Socket::init(int sock) {
	m_sock = sock;
	m_is_connected = true;
	init_sock();

	local_addr();
	remote_addr();

	return false;
}

Socket::ptr Socket::accept() {
	Socket::ptr sock(new Socket(m_family, m_type, m_protocol));
	int newsock = ::accept(m_sock, nullptr, nullptr);
	if(newsock == -1) {
		TT_DEBUG << "accept(" << m_sock 
			<< ") errno = " << errno 
			<< " errstr = " << strerror(errno);
		return nullptr;
	}
	if(sock->init(newsock)) {
		return sock;
	}
	return nullptr;
}
bool Socket::bind(const Address::ptr addr) {
	if(!is_valid()) {
		new_sock();
	}

	if(addr->family() != m_family) {
		TT_DEBUG << "bind sock.family(" << m_family 
			<< ") addr.family("<< addr->family() 
			<< ") not equal, addr = " << addr->to_string();
		return false;
	}

	// unix bind;
	if(::bind(m_sock, addr->addr(), addr->addr_len())) {
		TT_DEBUG << "bind error errno = " << errno 
				 << " errstr = " << strerror(errno);
		return false;
	}

	local_addr();
	return true;
}
bool Socket::connect(const Address::ptr addr) {
	if(!is_valid()) {
		new_sock();
		if(!is_valid()) {
			TT_DEBUG << "can^t new_sock!";
			return false;
		}
	}
	
	if(addr->family() != m_family) {
		TT_DEBUG << "bind sock.family(" << m_family 
			<< ") addr.family("<< addr->family() 
			<< ") not equal, addr = " << addr->to_string();
		return false;
	}

	if(::connect(m_sock, addr->addr(), addr->addr_len())) {
		TT_DEBUG << "sock = " << m_sock
				<< " connect(" << addr->to_string() << ")"
				<< " errno = " << errno 
				<< " errstr = " << strerror(errno);
		close();
		return false;
	}

	m_is_connected = true;
	remote_addr();
	local_addr();
	return true;
}
bool Socket::reconnect() {
	if(!m_remote_addr) {
		TT_DEBUG << "reconnected m_remote_addr is null";
		return false;
	}
	m_local_addr.reset();
	return connect(m_remote_addr);
}
bool Socket::listen(int backlog) {
	if(!is_valid()) {
		TT_DEBUG << "listen error sock = -1";
		return false;
	}
	if(::listen(m_sock, backlog)) {
		TT_DEBUG << "listen error errno = " << errno
				<< " errstr = " << strerror(errno);
		return false;
	}
	return true;
}

bool Socket::close() {
	if(!m_is_connected && m_sock == -1) { return true; }
	m_is_connected = false;
	if(m_sock != -1) {
		::close(m_sock);
		m_sock = -1;
		return true;
	}
	return false;
}

int Socket::send(const void* buffer, size_t len, int flags) {
	if(is_connected()) {
		return ::send(m_sock, buffer, len, flags);
	}
	return -1;

}

int Socket::send_to(const void* buffer, size_t len, const Address::ptr to, int flags) {
	if(is_connected()) {
		return ::sendto(m_sock, buffer, len, flags, to->addr(), to->addr_len());
	}	
	return -1;

}

int Socket::recv(void* buffer, size_t len, int flags) {
	if(is_connected()) {
		return ::recv(m_sock, buffer, len, flags);
	}
	return -1;

}
int Socket::recv_from(void* buffer, size_t len, const Address::ptr to, int flags) {
	if(is_connected()) {
		return ::recvfrom(m_sock, buffer, len, flags, to->addr(), &to->addr_len());
	}	
	return -1;

}

std::ostream& Socket::dump(std::ostream& os) const {
	os << "[Socket sock=" << m_sock
		<< " is_connected=" << m_is_connected
		<< " family=" << m_family
		<< " type=" << m_type
		<< " protocol=" << m_protocol;
	if(m_local_addr) {
		os << " local_addr=" << m_local_addr->to_string();
	}
	if(m_remote_addr) {
		os << " remote_addr=" << m_remote_addr->to_string();
	}
	os << "]";
	return os;
}
std::string Socket::to_string() const {
	std::stringstream ss;
	dump(ss);
	return ss.str();
}

Address::ptr Socket::remote_addr() {
	if(m_remote_addr) {
		return m_remote_addr;
	}

	Address::ptr result = AddressManager::NewAddress(m_family);

	socklen_t addrlen = result->addr_len();
	if(getpeername(m_sock, result->addr(), &addrlen)) {
		TT_DEBUG << "getpeername error sock=" << m_sock;
		return Address::ptr(new UnknownAddress(m_family));
	}
	m_remote_addr = result;
	return m_remote_addr;

}
Address::ptr Socket::local_addr() {
	if(m_local_addr) {
		return m_local_addr;
	}

	Address::ptr result = AddressManager::NewAddress(m_family);

	socklen_t addrlen = result->addr_len();
	if(getsockname(m_sock, result->addr(), &addrlen)) {
		TT_DEBUG << "getsockname error sock=" << m_sock;
		return Address::ptr(new UnknownAddress(m_family));
	}
	m_local_addr = result;
	return m_local_addr;

}


} // namespace net

} // namespace tt

