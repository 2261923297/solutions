#include "AutoMakeSrc.h"
#include <iostream>

std::string
ContextMaker::s_dependDir = "/";

void test_base() {
	std::cout << "test_base;" << std::endl;
	AutoMakeSrc::ptr amsr(new AutoMakeSrc);
	ContextMaker::ptr cm(new SrcContextMaker);
	ContextMaker::SetProjectName("test_pjt");
	ContextMaker::SetDependDir("~/cpp/depend/");
#define xx(class_name, maker_arg, file_path) \
	ContextMaker::ptr maker_arg(new class_name);	\
	amsr->addMaker(#file_path, maker_arg);			\
//
	xx(HeadContextMaker, h_cm, xx.h)
	xx(SrcContextMaker, s_cm, xx.cc)
	xx(TestContextMaker, t_cm, test_xx_func.cc)
	xx(MakeContextMaker, m_cm, mk.mk)
#undef xx // xx(class_name, maker_arg, file_path)

//	amsr->showStrings();
//	std::cout << "[xx.h]\n" << amsr->getContext("xx.h");
//	std::cout << "[xx.cc]\n" << amsr->getContext("xx.cc");
//	std::cout << "[test_xx_func.cc]\n" << amsr->getContext("test_xx_func.cc");
	std::cout << "[mk.mk]\n" << amsr->getContext("mk.mk");
	
}

int main() {
	std::cout << "Hello AutoMakeSrc!" << std::endl;
	test_base();

	return 0;
}

