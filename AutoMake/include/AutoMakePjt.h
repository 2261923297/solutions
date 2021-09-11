#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <memory>

class AutoMakePjt {
public:
	using ptr = std::shared_ptr<AutoMakePjt>;
	AutoMakePjt() { }
	virtual ~AutoMakePjt() { }

	void 
	setPjtName(const std::string& pjt_name) { m_pjt_name = pjt_name; }

	void 
	setPjtDir(const std::string& pjt_dir) { m_pjt_dir = pjt_dir; }

	const std::string&
	getPjtName() const { return m_pjt_name; }

	const std::string&
	getPjtDir() const { return m_pjt_dir; }

	void 
	addFile(const std::string& fileName) { m_files.push_back(fileName); }

	void
	delFile(const std::string& fileName)  {
		auto del_file = m_files.begin();
		for(; del_file != m_files.end(); del_file++) {
			if(*del_file == fileName) { break; }
		}
		if(del_file != m_files.end()) 
			m_files.erase(del_file);
	}

	void 
	showData()  {
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
	

protected:
	std::string				 m_pjt_dir;
	std::string				 m_pjt_name;
	std::vector<std::string> m_files;

}; // AutoMakePjt
