#include "AutoMakePjt.h"
#include <sstream>


void 
AutoMakePjt::delFile(const std::string& fileName) {
	auto del_file = m_files.begin();
	for(; del_file != m_files.end(); del_file++) {
		if(*del_file == fileName) { break; }
	}
	if(del_file != m_files.end()) 
		m_files.erase(del_file);
}

void
AutoMakePjt::showData() {
	std::stringstream ss;
	ss  << "AutoMakePjt:" 
		<< "\n\tm_pjt_dir=" << m_pjt_dir
		<< "\n\tm_pjt_name=" << m_pjt_name
		<< "\n\tm_files:";

	for(auto file : m_files) {
		ss << file << ", ";
	}
	ss << std::endl;
	std::cout << ss.str();

}
