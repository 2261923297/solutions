#include "structrue.h" 
#include "../../include/Log.h"
using namespace tt;
void
test_base() { 
	TT_DEBUG << "";
	Array<int>::ptr array(new Array<int>);
	for(int i = 0; i < 10; i++) {
		array->add(i);
	}
	std::stringstream ss;
	for(int i = 0; i < 10; i++) {
		ss << (*array)[i];
	}
	TT_DEBUG << ss.str();
}



int
main(int argc, char** argv) {
	TT_DEBUG << "Hello, structrue!" << std::endl;
	test_base();

	return 0; 
}
