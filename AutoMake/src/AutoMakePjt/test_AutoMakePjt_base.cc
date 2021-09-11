#include "AutoMakePjt.h"

void 
test_base() {

	AutoMakePjt::ptr amp(new AutoMakePjt);
	amp->setPjtName("test");
	amp->setPjtDir("xx/src");

	amp->addFile("test.h");
	amp->addFile("test.cc");
	amp->addFile("test_test_base.cc");
	amp->addFile("mk.mk");
	amp->addFile("mk");

	amp->showData();
	amp->delFile("mk");
	amp->showData();
}

int main() {
	std::cout << "Hello, AutoMakePjt" << std::endl;
	test_base();

	return 0;
}
