#include <iostream>
#define ENUM_CLASS_NAME Test
#define ENUM_NAME test
#define ENUM_BODY \
	ENUM_MESSAGE(test, red, black, yellow)
#define ENUM_VALUES \
	xx(Test::test::red) \
	xx(Test::test::black) \
	xx(Test::test::yellow) \
	//
#include "EnumReflection.h"

int 
main() {
	std::cout << Test::test::red << std::endl;

	std::cout << Test::test::black << std::endl;

	std::cout << Test::test::yellow << std::endl;

	return 0;
}
