#include "AutoMakeSrc.h"

std::string ContextMaker::s_projectName = "AutoMakeSrc";
std::string ContextMaker::s_projectPath = "/tt/cpp/solutions/AutoMake/src/";
std::string ContextMaker::s_binDir = "../../bin/";
	
const std::string
ContextMaker::getContext() { 
	makeContext(); 
	return m_ss.str(); 
}

#define xx(class_name, func_name)		\
void class_name::func_name()			\
//
xx(SrcContextMaker, makeContext) {
	m_ss << "#include \"" << ContextMaker::GetProjectName() << ".h\" " << "\n\n"
		<<  ContextMaker::GetProjectName() << "::" <<  ContextMaker::GetProjectName() << "() { " << "\n\n\n"
		<< "}" << "\n\n"
		<<  ContextMaker::GetProjectName() << "::~" <<  ContextMaker::GetProjectName() << "() {" << "\n\n\n"
		<< "}" << "\n\n"
		<< std::endl;
}

xx(HeadContextMaker, makeContext) {
	m_ss << "#pragma once" << "\n\n" 
		<< "#include <iostream>" << "\n"
		<< "#include <memory>"    << "\n"
		<< "#include <string>"	  << "\n\n"
		<< "class " <<  ContextMaker::GetProjectName() << "{ " << "\n"
		<< "public: " << "\n"
		<< "\tusing ptr = std::shared_ptr<" <<  ContextMaker::GetProjectName() << ">;" << "\n"
		<< "\t" <<  ContextMaker::GetProjectName() << "();" << "\n"
		<< "\t~" <<  ContextMaker::GetProjectName() << "();" << "\n\n\n" 
		<< "protected:" << "\n\n"
		<< "}; // " <<  ContextMaker::GetProjectName() << "\n\n"
		<< std::endl;
}

xx(TestContextMaker, makeContext) {
	m_ss << "#include \"" << ContextMaker::GetProjectName() << ".h\" " << "\n\n"
		<< "void\n" << "test_base() { \n\n"
		<< "}\n\n\n"
		<< "int\n" << "main(int argc, char** argv) {\n"
		<< "\tstd::cout << \"Hello, " << ContextMaker::GetProjectName() << "!\" << std::endl;\n"
		<< "\ttest_base();\n\n"
		<< "\treturn 0; \n}"
		<< std::endl;
}

xx(MakeContextMaker, makeContext) {

	std::string test_file_name = "test_" + ContextMaker::GetProjectName() + "_func.cc";
	m_ss << "#!/bin/bash" << "\n\n"
		<< "g++ " << ContextMaker::GetProjectName() << ".cc   \\" << "\n"
		<< "    " << test_file_name << "  \\" << "\n"
		<< "-o " << s_binDir << test_file_name << "\n\n\n"
		<< "chmod a+x " << s_binDir << test_file_name << " \n\n"
		<< s_binDir << test_file_name << "\n"
		<< std::endl;
}

#undef xx // xx
