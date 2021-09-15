#include "AutoMakeSlt.h"

#define ENUM_CLASS_NAME test
#define ENUM_NAME color
#define ENUM_BODY ENUM_MESSAGE(color, RED, BLACK, YELLOW)
#define ENUM_VALUES \
	xx(test::color::RED) \
	xx(test::color::BLACK) \
	xx(test::color::YELLOW) \
//
#include "../../include/EnumReflection.h"

/*
class test {
public:
	enum color { RED, BLACK, YELLOW };
};
*/
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
//	std::cout << "Hello! test AutoMakeSlt" << std::endl;
	test::color e =  test::color::BLACK;
	std::cout << e << std::endl;
//	test_base();

	return 0;
}
