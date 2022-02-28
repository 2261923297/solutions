#include "Fiber.h" 
#include "../../include/macro.h"
#include "../../include/Log.h"

void
test_assert() { 
	TT_ASSERT(0);
	TT_DEBUG << "test_tt_debug";
}
void fiber() {
	std::cout << " main_in fiber\n";
}
void test_base() {
	tt::Fiber f1(fiber);
	std::cout << "test_base\n";
	f1.swapin();
	std::cout << "swap in end\n";
	std::cout << "test_base\n";

}

int
main(int argc, char** argv) {
	tt::Fiber::GetThis();
	test_base();

	return 0; 
}
