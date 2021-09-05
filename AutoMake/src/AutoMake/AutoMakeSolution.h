#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "../../File/TFile.h"
#include "../../File/TWinFile.h"

class AutoMakeSolution {
public:
	typedef std::shared_ptr<AutoMakeSolution> ptr;

	AutoMakeSolution(const std::string rootDir = "D:\\Project\\cpp\\mSolutions\\"
				   , const std::string solutionName = "DefaultSolution");
	~AutoMakeSolution();

	void addProject(const std::string projectName) { m_projects.push_back(projectName); }

	void addDir(const std::string dirName) { m_dirs.push_back(dirName); }

	int run();

	std::string getSolutionName() const { return m_solutionName; }

	std::string getRootDir() const { return m_rootDir; }

	std::string getSolutionPath() const;

protected:

	virtual int initDir();

	virtual int initProject(const std::string projectName);




private:
	TFile::ptr  m_fileTool;

	std::string m_solutionName;	    //only name

	std::string m_rootDir;			//abs path

	std::vector<std::string> m_dirs;

	std::vector<std::string> m_projects;

}; // AutoMakeSolution





