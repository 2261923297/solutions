#include "../AutoMake.h"
#include <stdio.h>
void mkSolution(std::string solution_path, std::string solution_name) {
	AutoMakeSolution::ptr ams = AutoMakeSolution::ptr(new AutoMakeSolution(solution_path, solution_name));
	ams->run();
}
void mkProject(std::string project_path, std::string project_name) {
	AutoMakeProject::ptr amp = AutoMakeProject::ptr(new AutoMakeProject(project_path, project_name));
	AutoMakeSrc ams(project_name);

	amp->run();

	std::string head_str = "";
	std::string src_str = "";
	std::string mk_str = "";
	std::string test_str = "";

	ams.mkHead(head_str);
	ams.mkSrc(src_str);
	ams.mkTbase(test_str);
	ams.mkMk(mk_str);

	FILE* f_head, *f_src, *f_mk, *f_test;
	f_head = f_src = f_mk = f_test = nullptr;

	f_head = fopen(amp->getHeadPath().c_str(), "wb+");
	f_src = fopen(amp->getSrcPath().c_str(), "wb+");
	f_mk = fopen(amp->getMkPath().c_str(), "wb+");
	f_test = fopen(amp->getTestPath().c_str(), "wb+");

#define xx(f_, str) fwrite(str.c_str(), 1, str.size(), (FILE*)(f_) ) 
	xx(f_head, head_str);
	xx(f_src, src_str);
	xx(f_mk, mk_str);
	xx(f_test, test_str);
#undef xx
	
	fclose(f_head);
	fclose(f_src);
	fclose(f_mk);
	fclose(f_test);
}
// -p -s -?
void opt_trans(std::string opt, std::string rootPath, std::string name) {
	if(opt == "-s") {
		mkSolution(rootPath, name);	

	} else if(opt == "-p") {
		mkProject(rootPath, name);

	} else if(opt == "-?") {
		std::cout << "\t -s [solutionAbsPath] [solutionName]"
			<< "\n\t -p [projectAbsPath] [projectName]";
	} else {
		std::cout << "cant find operator for " << opt << std::endl;
	}

}
int main(int argc, char** argv) {
	std::cout << "Hello, AutoMake!" << std::endl;
	if(argc != 4) {
		std::cout << "error use of \"" << argv[0] 
			<< "\", please use \"" << argv[0] << " -?\" to make sure the using";
		
		return -1;
	}
	
	opt_trans(argv[1], argv[2], argv[3]);

	return 0;
}
