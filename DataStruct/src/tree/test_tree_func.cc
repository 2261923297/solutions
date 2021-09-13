#include "tree.h" 

using namespace tt;

std::vector<std::pair<int, std::string> > f_yaml_data::s_ori_strs;


void init(const std::string& file_path) {


}

void
test_base() { 
	ltree_node<f_yaml_data>::ptr ln(new ltree_node<f_yaml_data>("test"));
	
	
}


int
main(int argc, char** argv) {
	std::cout << "Hello, tree!" << std::endl;
	test_base();
	
	return 0; 
}
