#include "AutoMakeSolution.h"


AutoMakeSolution::AutoMakeSolution(const std::string rootDir, const std::string rootName) {
	m_rootDir = rootDir;
	m_solutionName = rootName;
	m_fileTool = TFile::ptr(new TWinFile(rootDir + rootName + '\\'));
	
	m_fileTool->formateDirName(m_rootDir);

}

AutoMakeSolution::~AutoMakeSolution() {

}



int AutoMakeSolution::run() {
	int ans = 0;
	initDir();
	std::string solution_dir = m_rootDir + m_solutionName;
	m_fileTool->formateDirName(solution_dir);

	for(auto str : m_projects) {
		initProject(str);
	}

	for(auto dir : m_dirs) {
		m_fileTool->createDir((solution_dir + dir).c_str());
	}

	m_dirs.clear();
	return 0;
}
std::string AutoMakeSolution::getSolutionPath() const {
	std::string ans = m_rootDir + m_solutionName;
	m_fileTool->formateDirName(ans);
	return ans;
}

int AutoMakeSolution::initDir() {

	addDir("bin");
	addDir("tests");
	addDir("doc");
	addDir("src");
	addDir("include");
	addDir("lib");
	
	return 0;
}

int AutoMakeSolution::initProject(const std::string projectName) {
	std::string src_dir = "src";
	m_fileTool->formateDirName(src_dir);
	addDir(src_dir + projectName);
	
	return 0;
}


