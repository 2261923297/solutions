#include "structrue.h" 
#include "../../include/Log.h"
using namespace tt;
void
test_base() { 
	TT_DEBUG << "";
	structrue<int>::ptr array(new Array<int>);

	for(int i = 0; i < 10; i++) {
		array->add(i);
		TT_DEBUG << i 
			<< ", array size: " << array->size() 
			<< ", array capacity: " << array->capacity();
	
	}

	std::stringstream ss;
	for(int i = 0; i < 10; i++) {
		ss << (*array)[i];
	}
	TT_DEBUG << ss.str();
}



int
main(int argc, char** argv) {
	test_base();

	return 0; 
}
