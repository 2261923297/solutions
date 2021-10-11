#include "ResourceMapper.h" 

using namespace tt::FileResource;
void
test_base() { 
	tt::FileResource::Manager::ptr manager(new Manager);
	manager->add("test");
	manager->get("test");
	manager->list("test");
	manager->del("test");
	manager->reset("test");

}


int
main(int argc, char** argv) {
	std::cout << "Hello, ResourceMapper!" << std::endl;
	test_base();

	return 0; 
}
