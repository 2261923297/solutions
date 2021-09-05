#include "../AutoMakeSolution.h"

int test_construct() {
	AutoMakeSolution::ptr amkP = AutoMakeSolution::ptr(new AutoMakeSolution("E:\\AutoMakeTest"));

	return 0;
}


void test_add_init_run() {
	AutoMakeSolution::ptr amkP = AutoMakeSolution::ptr(new AutoMakeSolution);
	
	amkP->addProject("Logger");
	amkP->addProject("Configer");

	amkP->run();
 
}


int main() {
	std::cout << "Hello, amk_test_white!" << std::endl;

	//test_construct();

	test_add_init_run();
	return 0;
}
