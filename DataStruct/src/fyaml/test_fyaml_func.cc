#include "fyaml.h" 

void
test_base() { 
	std::cout << fyaml_type::type::MAP << std::endl;
}


int
main(int argc, char** argv) {
	std::cout << "Hello, fyaml!" << std::endl;
	test_base();

	return 0; 
}
