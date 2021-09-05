#pragma once
#include <iostream>
#include <memory>
#include <vector>

class AutoMakeSrc {
public:
	typedef std::shared_ptr<AutoMakeSrc> ptr;
	AutoMakeSrc(const std::string& projectName) : m_projectName(projectName) {

	}
	~AutoMakeSrc() {

	}

	void setProjectName(const std::string& projectName) { m_projectName = projectName; }

	std::string getProjectName() { return m_projectName; }

	int mkTbase(std::string& str);

	int mkHead(std::string& str);

	int mkSrc(std::string& str);

	int mkMk(std::string& str);

protected:
	int mkstr(const std::vector<std::string>& str_pools, std::string& str);

private:
	std::string m_projectName;
	const static std::vector<std::string> s_head;
	const static std::vector<std::string> s_src;
	const static std::vector<std::string> s_mk;
	const static std::vector<std::string> s_tbase;

	const static std::string s_newLineStr;
};

