#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>

class AutoMakeSlt {
public:
	using ptr = std::shared_ptr<AutoMakeSlt>;
	AutoMakeSlt() { }
	virtual ~AutoMakeSlt() { }

	void 
	addDir(const std::string& dir_name) { m_dirs.push_back(dir_name); }

	void 
	delDir(const std::string& dir_name)  {
		auto del_iter = m_dirs.begin();
		for(; del_iter != m_dirs.end(); del_iter++) {
			if(*del_iter == dir_name) {
				break;
			}
		}
		if(del_iter != m_dirs.end()) 
			m_dirs.erase(del_iter);
	}

	const std::vector<std::string>&
	getDirs() { return m_dirs; }	

	void 
	clearDir() { m_dirs.clear(); }

	void
	setRootDir(const std::string& root_name) { m_root_dir = root_name; }

	void 
	setSltName(const std::string& slt_name) { m_slt_name = slt_name; }

	const std::string&
	getRootDir() { return m_root_dir; }

	const std::string&
	getSltName() { return m_slt_name; }

	void
	showData()  {
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
protected:
	std::vector<std::string> m_dirs;
	std::string m_root_dir;
	std::string m_slt_name;
}; // classs AutoMakeSlt
