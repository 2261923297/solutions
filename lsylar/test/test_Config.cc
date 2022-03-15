#include "Config.h" 

using namespace tt::system;

Config::config_map_t Config::s_datas;
Config::mutex_t Config::s_mutex;
void
test_base() { 

	ConfigVarBase::ptr c_port = 
		Config::LookUp<int>("port"
		, 8080, "ipv4 port");


	std::cout << c_port->to_string() << std::endl;
}


int
main(int argc, char** argv) {
	std::cout << "Hello, Config!" << std::endl;
	test_base();

	return 0; 
}


















