#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "../../File/TWinFile.h"

class AutoMakeProject {
public:
	typedef std::shared_ptr<AutoMakeProject> ptr;
	AutoMakeProject(const std::string& rootPath, const std::string& projectName);

	~AutoMakeProject();

	const std::string& getRootPath() {	return m_rootPath; }

	const std::string& getprojectName() { return m_projectName;	}

	void setRootPath(const std::string& rootPath) {	m_rootPath = rootPath; }

	void setProjectName(const std::string& projectName) { m_projectName = projectName; }

	std::string getProjectPath() {
		std::string ans = m_rootPath + m_projectName;
		m_fileTool->formateDirName(ans);
		return ans;
	}
	std::string getHeadPath() {	return getProjectPath() + m_projectName + ".h";	}

	std::string getSrcPath() { return getProjectPath() + m_projectName + ".cc";	}

	std::string getMkPath() { return getProjectPath() + "mk"; }

	std::string getTestPath() {	return getProjectPath() + std::string("test_") + m_projectName + "_white.cc"; }
	int run(const std::string& projectName = "");


protected:
	std::string init();      //return projectPath, initProjectFile(); 


	virtual int initProjectFile(); //%p.h, %p.cc, mk, test_%p_white.cc
protected:
	TFile::ptr m_fileTool;

	std::string m_rootPath;
	std::string m_projectName;

	std::vector<std::string> m_files;
};
