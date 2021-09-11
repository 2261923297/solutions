#include <iostream>
#include <string>
#include <memory>
#include "../../include/Log.h"
#include <vector>

class AutoMakeSlt {
public:
	using ptr = std::shared_ptr<AutoMakeSlt>;
	AutoMakeSlt() { }
	virtual ~AutoMakeSlt() { }

	void 
	addDir(const std::string& dir_name) { m_dirs.push_back(dir_name); }

	void 
	delDir(const std::string& dir_name);

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
	showData();
protected:
	std::vector<std::string> m_dirs;
	std::string m_root_dir;
	std::string m_slt_name;
}; // classs AutoMakeSlt
