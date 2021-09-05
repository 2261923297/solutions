#include <iostream>
#include <memory>
#include <string.h>
#include "AutoMakeSrc.h"

const std::string AutoMakeSrc::s_newLineStr = "\r\n";

const std::vector<std::string> AutoMakeSrc::s_head = {
		  "#pragma once"
		, "#include <iostream> "
		, "#include <memory>"
		, "  "
		, "class %p {"
		, "public: "
		, "    typedef std::shared_ptr<%p> ptr; "
		, "    "
		, "    %p(); "
		, "    virtual ~%p();"
		, "    "
		, "    "
		, "private: "
		, "    "
		, "}; // %p"
		, "    "
};


const std::vector<std::string> AutoMakeSrc::s_src = {
		  "#include \"%p.h\""
		, "#include <string>"
		, "    "
		, "%p::%p() {"
		, "    "
		, "}"
		, "    "
		, "%p::~%p() {"
		, "    "
		, "}"
		, "    "

};



const std::vector<std::string> AutoMakeSrc::s_mk = {
		  "#!/bin/bash"
		, "    "
		, "clear "
		, "    "
		, "g++ %p.cc \\"
		, "    test_%p_white.cc \\"
		, "-o ../../bin/test_%p_white.exe"
		, "    "
		, "../../bin/test_%p_white.exe"
		, "    "

};

const std::vector<std::string> AutoMakeSrc::s_tbase = {
			  "#include \"%p.h\""
			, "    "
			, "void test_shared_ptr() {"
			, "    "
			, "}"
			, "    "
			, "int main() {"
			, "    std::cout << \"Hello, test %p base!\" << std::endl;"
			, "    test_shared_ptr();"
			, "    "
			, "    return 0;"
			, "}"
			, "    "
};


int AutoMakeSrc::mkTbase(std::string& str) {
	mkstr(s_tbase, str);

	return 0;
}

int AutoMakeSrc::mkHead(std::string& str) {
	mkstr(s_head, str);

	return 0;
}

int AutoMakeSrc::mkSrc(std::string& str) {
	mkstr(s_src, str);

	return 0;
}

int AutoMakeSrc::mkMk(std::string& str) {
	mkstr(s_mk, str);

	return 0;
}

int AutoMakeSrc::mkstr(const std::vector<std::string>& str_pools, std::string& ans) {
	for(auto str : str_pools) {
		for(int i = 0; i < str.size(); i++) {
			if(str[i] == '%' && i + 1 < str.size() && str[i + 1] == 'p') {
				ans += m_projectName;
				i++;
				continue;
			}
			ans += str[i];
		}
		ans += s_newLineStr;
	}
	return 0;
}






