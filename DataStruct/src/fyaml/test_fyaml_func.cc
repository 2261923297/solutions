#include "fyaml.h" 
#include "../../include/File.h"


void fyaml_from_file(const std::string& file_name, std::vector<int>& levels, std::vector<std::string>& strs) {
	tt::File::Entry::ptr fentry(new tt::File::Entry(file_name));	
	fentry->reopen();
	const size_t buffer_size = 2048;
	char buffer[buffer_size + 1] = { 0 };
	fentry->read(buffer, buffer_size);
//	std::cout << buffer << std::endl;

	// split line
	int line = 0;
	levels.push_back(0);
	strs.push_back("");
	for(int i = 0; i < buffer_size; i++) {
		if(buffer[i] == '\t') {
			levels[line]++;
			continue;
		} else if(buffer[i] == '\n') {
			if(buffer[i - 1] == '\n') {continue; }
			line ++;
			levels.push_back(0);
			strs.push_back("");
			continue;
		}
		strs[line] += buffer[i];
	}
	
}

void 
make_fyamls(tt::fyaml_data::ptr& fyaml_root, 
		std::vector<int>& levels, 
		std::vector<std::string>& strs) {
	int max_level = levels[0];
	for(auto level : levels) 
	  { max_level = max_level > level ? max_level : level; }	

}
void
test_base() { 
	tt::fyaml_data::ptr fd (new tt::fyaml_data);
	std::cout << fyaml_type::type::MAP << std::endl;
	std::vector<int> levels;
	std::vector<std::string> strs;

	fyaml_from_file("./fyaml.conf", levels, strs);
	int line = levels.size();
	for(int i = 0; i < line; i++) {
		std::cout << i << ": " << levels[i] << ", " << strs[i] << std::endl;
	}

	fd->load(levels, strs, 0, 6);
	fd->show_data();
	std::cout << "type: " << fd->type() << std::endl;
#undef xx
#define xx(beg, nLine) \
	fd.reset(new tt::fyaml_data); \
	fd->load(levels, strs, beg, nLine); \
	fd->show_data();\
	//
	xx(6, 4)
	xx(10, 1)
	xx(11, 1)
	xx(12, 1)
#undef xx // xx(beg, nLine)
}


int
main(int argc, char** argv) {
	std::cout << "Hello, fyaml!" << std::endl;
	test_base();

	return 0; 
}
