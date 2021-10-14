#include <iostream>
#include <string>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <map>
#include "Log.h"
namespace tt {
namespace net {

class Address {
public:
	using ptr = std::shared_ptr<Address>;
	Address() { }
	virtual ~Address() { }

	int family() { return addr()->sa_family; }

	virtual sockaddr* addr() = 0;
	virtual socklen_t& addr_len() = 0;
	
	virtual std::ostream& insert(std::ostream& os) const = 0;

	std::string to_string() const { 
		std::stringstream ss;
		insert(ss);
		return ss.str(); 
	}

protected:
	
}; // class Address;

class IPAddress : public Address {
public:
	using ptr = std::shared_ptr<IPAddress>;
	static IPAddress::ptr Create(const char* address, uint16_t port = 0);
	virtual IPAddress::ptr broadcast_addr(uint32_t prefix_len) = 0;
	virtual IPAddress::ptr network_addr(uint32_t prefix_len) = 0;
	virtual IPAddress::ptr subnet_mask(uint32_t prefix_len) = 0;
	virtual uint32_t port() const = 0;
	virtual void set_port(uint16_t v) = 0;
private:

}; // IPAddress 

class IPv4Address : public IPAddress {
public:

	using ptr = std::shared_ptr<IPv4Address>;
	static IPv4Address::ptr Create(const char* address, uint16_t port = 0);
	IPv4Address(const sockaddr_in& address);
	IPv4Address(uint32_t address = INADDR_ANY, uint16_t port = 0);

	sockaddr* addr() override;
	socklen_t& addr_len() override;
	std::ostream& insert(std::ostream& os) const override;

	IPAddress::ptr broadcast_addr(uint32_t prefix_len) override;
	IPAddress::ptr network_addr(uint32_t prefix_len) override;
	IPAddress::ptr subnet_mask(uint32_t prefix_len) override;
	uint32_t port() const override;
	void set_port(uint16_t v) override;

private:
	sockaddr_in m_addr;
	socklen_t m_len;
};;
class UnknownAddress : public Address {
public:
	UnknownAddress(int family) { m_addr.sa_family = family; }
	virtual sockaddr* addr() override { return &m_addr; }
	virtual socklen_t& addr_len() override { return m_len; }
	
	virtual std::ostream& insert(std::ostream& os) const override { os << "UnknownAddress::insert";  return os;}
	
	

private:
	sockaddr m_addr;
	socklen_t m_len;
}; // UnknownAddress
class AddressManager {
public:
	static Address::ptr NewAddress(int family);

private:
}; // AddressManager;

} // namespace TNet
} // namespace tt
