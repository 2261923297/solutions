#include "Fiber.h" 
#include "macro.h"
#include "Log.h"

void
test_assert() { 
	TT_ASSERT(0);
	TT_DEBUG << "test_tt_debug";
}
void fiber() {
	std::cout << " main_in fiber\n";
//	tt::system::Fiber::GetThis()->swapout();
	tt::system::Fiber::GetThis()->YeldToReady();
//	tt::system::Fiber::GetThis()->YeldToHold();
}

/**
 *	test_base
 *
 *	main_in_fiber
 *
 *	swap in end 
 *
 *	test_base
 *
 * */

void txt1() {
	std::cout << "txt1" << std::endl;
	tt::system::Fiber::GetThis()->swapout();
}

void txt2() {
	std::cout << "txt2" << std::endl;

}



void test_base() {
	TT_DEBUG << "test_base";
	tt::system::Fiber f1(fiber);
	tt::system::Fiber f2(txt1);
	tt::system::Fiber f3(txt2);
	f1.swapin();
	f2.swapin();
	f3.swapin();

}

int
main(int argc, char** argv) {
	tt::system::Fiber::GetThis();
	test_base();

	return 0; 
}
