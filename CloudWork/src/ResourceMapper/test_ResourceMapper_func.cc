#include "ResourceMapper.h" 
const std::string tt::FileResource::Mapper::s_root_dir = "/";

using namespace tt::FileResource;
void
test_base() { 
	tt::FileResource::Manager::ptr manager(new Manager);
	std::vector<std::string> works = {"Add /usr", 
		"Del /usr",
		"Get /usr",
		"List /usr",
		"Update /usr"
	};
	for(auto work : works) {

		manager->schedul(work);
	}
}

int
main(int argc, char** argv) {
	Manager::InitWorkerMapper();
	std::cout << "Hello, ResourceMapper!" << std::endl;
	test_base();

	return 0; 
}
