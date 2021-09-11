#include "AutoMakeSlt.h"
#include <sstream>

void
AutoMakeSlt::delDir(const std::string& dirName) {
	auto del_iter = m_dirs.begin();
	for(; del_iter != m_dirs.end(); del_iter++) {
		if(*del_iter == dirName) {
			break;
		}
	}
	if(del_iter != m_dirs.end()) 
		m_dirs.erase(del_iter);

}
void 
AutoMakeSlt::showData() {

	std::stringstream ss;
	ss << "AutoMakeSlt: " 
		<< "\n\tm_root_dir=" << m_root_dir
		<< "\n\tm_slt_name=" << m_slt_name
		<< "\n\tm_dirs:";
	for(auto dir : m_dirs) {
		ss << dir << ", ";
	}
	ss << std::endl;
	std::cout << ss.str();
}
