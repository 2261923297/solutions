#pragma once
#include "../../File/TFile.h"
#include "../../File/TWinFile.h"
#include "AutoMakeProject.h"
#include <memory>	

/**************************FIRST_DESING******************************/

//dirFormate: root/bin, root/project, root/tests, root/project/tests
	//fileFormate: project/(h + cc + mk_project), project/tests/(test_base.cc + mk_tProject)
	//r/b, p, t
	//
	//r/p/h,cc,twcc, p, t
	//r/t/tbcc,mk

	//--> rootDir/bin, rootDir/projectName, rootDir/tests
	//
	//--> --> rootDir/projectName/tests 
	//
	//--> --> rootDir/projectName/project.h + project.cc, rootDir/tests/test_projectName.cc
	//
	//--> --> --> rootDir/projectName/tests/test_base.cc

/**************************FIRST_DESIGN_END************************/



class AutoMakeSolution {
public:
	typedef std::shared_ptr<AutoMakeSolution> ptr;

	AutoMakeSolution(const std::string& rootDir);

	~AutoMakeSolution() {
		std::cout << "class AutoMakeSolution->root: " << m_rootDir << "is deleted!" << std::endl;
	}

	int addProjectName(const std::string& dirPath);

	int init(); 

	int initFileName(const std::string& dirName, const std::string& projectName);


	void showDirAndFile();
	
	int run();
private:
	TFile::ptr m_fileTool;
	
	std::string m_rootDir;
	std::string m_binDir;
	std::string m_testsDir;

	std::vector<std::string> m_projectsName;
	AutoMakeProject::ptr  m_projectTool;

	std::vector<std::string> m_initDirs;
	std::vector<std::string> m_initFiles;
};

