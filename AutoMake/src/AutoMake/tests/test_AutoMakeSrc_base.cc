#include "../AutoMakeSrc.h"
#include "../../../File/TWinFile.h"
#include "../../../File/TFile.h"

void test_base() {
	AutoMakeSrc::ptr ams = AutoMakeSrc::ptr(new AutoMakeSrc("TestProject"));
	std::string str_base = "";
	std::string str_head = "";
	std::string str_mk = "";
	std::string str_src = "";

	ams->mkTbase(str_base);
	ams->mkHead(str_head);
	ams->mkSrc(str_src);
	ams->mkMk(str_mk);

	std::cout << str_base << "\n"
		<< str_head << "\n"
		<< str_src << "\n"
		<< str_mk << "\n"
		<< std::endl;

	FILE* f_head = fopen("./TestProject/TestProject.h", "wb+");
	FILE* f_src = fopen("./TestProject/TestProject.cc", "wb+");
	FILE* f_mk = fopen("./TestProject/mk", "wb+");
	FILE* f_tbase = fopen("./TestProject/test_TestProject_base.cc", "wb+");

	perror("fopen four file of project_TestProject");
#define xx(str, file) \
	fwrite(str.c_str(), 1, str.size(), file)

	xx(str_base, f_tbase);
	xx(str_head, f_head);
	xx(str_src, f_src);
	xx(str_mk, f_mk);

}



int main() {
	std::cout << "Hello, AutoMakeSrc!" << std::endl;
	test_base();


	return 0;
}
