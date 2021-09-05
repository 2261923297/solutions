#include "File.h"

void test_base() {

	tt::File::Path p("./");
	p.showData();
	std::cout << "getcwd: " << p.current() << std::endl;

	std::vector<std::string> dirs;
	p.getdirs(dirs, "/tt/cpp/solution");
	std::cout << "dirs: ";
	for(auto name : dirs) {
		std::cout << name << ", ";
	}
	std::cout << "\n";
	
	std::cout << PLATFORM << std::endl;
	p.reset("../");
	p.showData();
}

void test_data(tt::File::Data::ptr d) {
	std::cout << "path: " << d->getPath()
		<< "\n\texit: "	<< d->exit()
		<< "\n\tisDir: " << d->isDir()
		<< "\n\tisReg: " << d->isReg()
		<< "\n\tsize: "  << d->size()
		<< std::endl;

}
void test_entry() {
	tt::File::Entry entry(tt::File::Path("/tt/cpp/solutions/PlatformModul/src/TFile/test_file.txt"));
		
	entry.getData()->showData();
	entry.getPath()->showData();
	entry.reopen("w");
	const size_t capa = 1025;
	size_t size = capa - 1;
	char buffer[capa] = { 0 };	
	for(int i = 0; i < size; i++) {
		if(i % 26 == 0) { buffer[i] = '\n'; continue; }
		buffer[i] = (i % 26) + 'a';
	}
	buffer[size] = 0;
	buffer[size - 1] = '\n';
	entry.write(buffer, size);
	std::cout << "pos: " << entry.getPos() << std::endl; 
	entry.setPos(10); // is cover, not insert new_pos = ori_pos + size;
	std::cout << "pos: " << entry.getPos() << std::endl;
	entry.write(buffer, size - 512);
	std::cout << "pos: " << entry.getPos() << std::endl;
	entry.close();
	entry.resetData();   // ??????????file_size 刷新不了！！！！
	entry.getData()->showData();
	
}
int main() {

//	test_base();
//	test_data();
	test_entry();
	return 0;

}
