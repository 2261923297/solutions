#include "Address.h"

namespace tt {
namespace net{


IPAddress::ptr 
IPAddress::Create(const char* address, uint16_t port) {

}

IPv4Address::ptr 
IPv4Address::Create(const char* address, uint16_t port) {
	IPv4Address::ptr rt(new IPv4Address);

	int result = inet_pton(AF_INET, address, &rt->m_addr.sin_addr);
	if(result < 0) {
		TT_DEBUG << "TPv4Address::Create(" << address << ", "
			<< port << ") rt = " << result << "errno = " << errno
			<< " errstr = " << strerror(errno);
		return nullptr;
	}
	rt->m_addr.sin_family = AF_INET;
	rt->m_addr.sin_port = htons(port);
	rt->m_len = sizeof(rt->m_addr);

	return rt;
}

IPv4Address::IPv4Address(const sockaddr_in& address) {
	m_addr = address;
	m_len = sizeof(address);
}

IPv4Address::IPv4Address(uint32_t address
		, uint16_t port) {

}

sockaddr* 
IPv4Address::addr()  {
	return (sockaddr*)&m_addr;

}

Address::ptr
AddressManager::NewAddress(int family) {
	Address::ptr addr;
	switch(family) {
		case AF_INET:
			addr.reset(new IPv4Address);
			break;
		default:
			addr.reset(new UnknownAddress(family));
	}
	return addr;
}

socklen_t& 
IPv4Address::addr_len()  {
	return m_len; 
}
std::ostream& 
IPv4Address::insert(std::ostream& os) const  {
	uint32_t addr = htonl(m_addr.sin_addr.s_addr);
	os << ((addr >> 24) & 0xff) << "."
		<< ((addr >> 16) & 0xff) << "."
		<< ((addr >> 8) & 0xff) << "."
		<< ((addr & 0xff));
	os << ":" << htons(m_addr.sin_port);
}

IPAddress::ptr 
IPv4Address::broadcast_addr(uint32_t prefix_len)  {

}

IPAddress::ptr 
IPv4Address::network_addr(uint32_t prefix_len)  {

}

IPAddress::ptr 
IPv4Address::subnet_mask(uint32_t prefix_len)  {

}

uint32_t
IPv4Address::port() const  {
	return m_addr.sin_port;
}

void 
IPv4Address::set_port(uint16_t v)  {
	m_addr.sin_port = htons(v);
}

}



}
