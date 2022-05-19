#include "File.h"
#include "Log.h"

void test_base() {
	tt::system::Path p("./");
	p.showData();
	TT_DEBUG << "getcwd: " << p.current() << std::endl;

	std::vector<std::string> dirs;
	p.getdirs(dirs, "/tt/cpp/solution");
	TT_DEBUG << "dirs: ";
	for(auto name : dirs) {
		std::cout << name << ", ";
	}
	std::cout << "\n";
	
	std::cout << PLATFORM << std::endl;
	p.reset("../");
	p.showData();
}

void test_data(tt::system::Data::ptr d) {
	std::cout << "path: " << d->getPath()
		<< "\n\texit: "	<< d->exit()
		<< "\n\tisDir: " << d->isDir()
		<< "\n\tisReg: " << d->isReg()
		<< "\n\tsize: "  << d->size()
		<< std::endl;

}
void test_entry() {
	tt::system::Entry entry(
		tt::system::Path(
			"/root/cpp/solutions/lsylar/bin/for_test"
		));
		
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

void test_FileManager() {
	tt::system::FileManager<tt::system::LinuxDirMaker> fm;
	fm.mkdir("./test_dir");
	fm.mkdirs("./111/222/333/444");

}
int main() {
	TT_DEBUG << "Hello, test_File_func!" << std::endl;
	test_base();

	TT_DEBUG << "test_entry: { ";
	test_entry();
	std::cout << "\n}\n\n";
	test_FileManager();	
	return 0;

}
