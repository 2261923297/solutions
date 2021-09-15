#include "fyaml.h" 
#include "../../include/File.h"


void fyaml_from_file(const std::string& file_name, std::vector<int>& levels, std::vector<std::string>& strs) {
	tt::File::Entry::ptr fentry(new tt::File::Entry(file_name));	
	fentry->reopen();
	const size_t buffer_size = 2048;
	char buffer[buffer_size + 1] = { 0 };
	fentry->read(buffer, buffer_size);
	std::cout << buffer << std::endl;

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

	fd.reset(new tt::fyaml_data);
	fd->load(levels, strs, 6, 4);
	fd->show_data();
}


int
main(int argc, char** argv) {
	std::cout << "Hello, fyaml!" << std::endl;
	test_base();

	return 0; 
}
