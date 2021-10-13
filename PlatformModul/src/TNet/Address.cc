#include "Address.h"

namespace tt {
namespace net{


static IPAddress::ptr 
IPAddress::Create(const char* address, uint16_t port = 0) {

}

static IPv4Address::ptr 
IPv4Address::Create(const char* address, uint16_t port = 0) {

}

IPv4Address::IPv4Address(const sockaddr_in& address) {
	m_addr = address;
}
IPv4Address::IPv4Address(uint32_t address = INADDR_ANY
		, uint16_t port = 0) {

}

sockaddr* 
IPv4Address::addr() override {

}
socklen_t& 
IPv4Address::addr_len() override {

}
std::ostream& 
IPv4Address::insert(std::ostream& os) const override {

}

IPAddress::ptr 
IPv4Address::broadcast_address(uint32_t prefix_len) override {

}
IPAddress::ptr 
IPv4Address::netword_address(uint32_t prefix_len) override {

}
IPAddress::ptr 
IPv4Address::subnet_mask(uint32_t prefix_len) override {

}
uint32_t
IPv4Address::port() const override {

}
void 
IPv4Address::set_port(uint16_t v) override {

}

}



}
