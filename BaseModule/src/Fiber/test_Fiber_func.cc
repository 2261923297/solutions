#include "Fiber.h" 
#include "../../include/macro.h"
#include "../../include/Log.h"

void
test_assert() { 
	TT_ASSERT(0);
	TT_DEBUG << "test_tt_debug";
}
void test_base() {



}

int
main(int argc, char** argv) {
	std::cout << "Hello, Fiber!" << std::endl;
	test_base();

	return 0; 
}
