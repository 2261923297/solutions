#include "../AutoMakeProject.h"

void test_white() {
	AutoMakeProject::ptr amp = AutoMakeProject::ptr(new AutoMakeProject("E:\\AutoMake\\SolutionDir\\", "Project"));
	amp->run();
}


int main() {
	
	test_white();
	std::cout << "Hello, ams_test_white" << std::endl;
	return 0;
}
