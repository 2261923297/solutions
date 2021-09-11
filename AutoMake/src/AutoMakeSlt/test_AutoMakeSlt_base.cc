#include "AutoMakeSlt.h"

void 
test_base() {
	AutoMakeSlt::ptr ams(new AutoMakeSlt);
	ams->setRootDir("/mnt/c/Users/win_template/Desktop/");
	ams->setSltName("test_dir");
	ams->addDir("bin");

	ams->addDir("sbin");
	ams->addDir("lib");
	ams->addDir("doc");
	ams->addDir("include");
	ams->addDir("src");
	ams->addDir("test");
	ams->showData();

	ams->delDir("sbin");
	std::cout << "del->sbin: " << std::endl;
	ams->showData();

}

int main() {
	std::cout << "Hello! test AutoMakeSlt" << std::endl;
	test_base();

	return 0;
}
