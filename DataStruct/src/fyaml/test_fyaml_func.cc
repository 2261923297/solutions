#include "fyaml.h" 
#include "data.h"
#include "../../include/File.h"

void 
make_fyamls(tt::fyaml_data::ptr& fyaml_root, 
		std::vector<int>& levels, 
		std::vector<std::string>& strs) {
	int max_level = levels[0];
	for(auto level : levels) 
	  { max_level = max_level > level ? max_level : level; }	

}

tt::fyaml_data::ptr 
mkdata(const std::vector<int>& levels, const std::vector<std::string>& strs, int beg, int nLines) {
	tt::fyaml_data::ptr ans(new tt::fyaml_data);
	ans->load(levels, strs, beg, nLines);
	return ans;
}
void
test_base(const std::string& file_name) { 
	tt::fyaml_data::ptr fd (new tt::fyaml_data);

	tt::fyaml_loader::ptr fl(new tt::fyaml_loader);
	fl->load_from_file(file_name);

	const std::vector<int>& levels = fl->levels();
	const std::vector<std::string>& strs = fl->confs();

	int line = levels.size();
	for(int i = 0; i < line; i++) {
		std::cout << i << ": " << levels[i] << ", " << strs[i] << std::endl;
	}
	tt::fyaml_data::ptr fds[5] = { nullptr };
#undef xx
#define xx(fd, beg, nLine) \
	fd.reset(new tt::fyaml_data); \
	fd->load(levels, strs, beg, nLine); \
	fl->add(fd); \
	fl->find(fds[0]->level(), fd->name())->show_data(); \
	fd->show_data();\
	//
	xx(fds[0], 6, 4)
//	xx(fds[1], 10, 1)
//	xx(fds[2], 11, 1)
//	xx(fds[3], 12, 1)
//	xx(fds[4], 0, 6)
#undef xx // xx(beg, nLine)
	fl->root()->show_data();		

}

void test_loader(const std::string& file_name) {
	tt::fyaml_loader::ptr fl(new tt::fyaml_loader);
	fl->load_from_file(file_name);
	tt::fyaml_data::ptr& root = fl->root();	
	
	const std::vector<int>& levels = fl->levels();
	const std::vector<std::string>& confs = fl->confs();
	std::cout << "----------every_line-------" << std::endl;
	for(int i = 0; i < levels.size(); i++) {
		std::cout << "i = " << i 
			<< ", level: " << levels[i] 
			<< ", " << confs[i] 
			<< std::endl;
	}
	
	std::cout << "-----------to_string--------" << std::endl;
	fl->auto_make_data();
//	root->show_data();
	std::stringstream ss;
	root->to_string(ss);

	std::cout << ss.str() << std::endl;
	
//	for()
	
}

void test_data() {
	tt::fyaml::data::ptr d(new tt::fyaml::data("test", ));
}
main(int argc, char** argv) {
//	test_base(argv[1]);
	test_loader(argv[1]);
	return 0; 
}








