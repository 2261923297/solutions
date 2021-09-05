#include "AutoMakeSolution.h"

AutoMakeSolution::AutoMakeSolution(const std::string& rootDir) {
	m_fileTool =TFile::ptr(new TWinFile("C://"));	

	m_rootDir = rootDir;
	m_fileTool->formateDirName(m_rootDir);
	
	m_projectTool = AutoMakeProject::ptr(new AutoMakeProject(m_rootDir, ".ams"));

	
	m_binDir = m_rootDir + "bin";
	m_testsDir = m_rootDir + "tests";
	m_fileTool->formateDirName(m_binDir);
	m_fileTool->formateDirName(m_testsDir);

	int mkRootDirRes = m_fileTool->createDir(m_rootDir.c_str());
	if(mkRootDirRes == -1) {
		std::cout << "AutoMkDirFile Mk rootDir error!" << std::endl;
	}
}
int AutoMakeSolution::addProjectName(const std::string& dirPath) {
	int ans = 0;
	m_projectsName.push_back(dirPath);
	return ans;
}

//dirFormate: root/bin, root/project, root/tests, root/project/tests
//fileFormate: project/(h + cc + mk_project), project/tests/(test_base.cc + mk_tProject)
//r/b, p, t
//
//r/p/h,cc,twcc, p, t
//r/t/tbcc,mk
int AutoMakeSolution::init() {
	m_initDirs.push_back(m_binDir);
	m_initDirs.push_back(m_testsDir);
	
	int ans = 0;
	std::string projectDir = "";
	for(auto i : m_projectsName) {
		m_initFiles.push_back(m_testsDir + "tb_" + i + ".cc");
		ans++;
	}
	showDirAndFile();
	return ans;
}


int AutoMakeSolution::initFileName(const std::string& dirName, const std::string& projectName) {
	m_initFiles.push_back(dirName + projectName + ".h");
	m_initFiles.push_back(dirName + projectName + ".cc");
	m_initFiles.push_back(dirName + "mk");

	return 3;
}

void AutoMakeSolution::showDirAndFile() {
	std::cout << "root: " << m_rootDir << " {" 
		<< "\n\tdirs: ";
	for(auto i : m_initDirs) {
		std::cout << "\n\t\t" << i;
	}
	std::cout << "\n\tfiles: " ;
	for(auto i : m_initFiles) {
		std::cout << "\n\t\t" << i;
	}

	std::cout << "\n} AutoMake_RootDir<" << m_rootDir << "> " << std::endl; 
}
//--> rootDir/bin, rootDir/projectName, rootDir/tests
//
//--> --> rootDir/projectName/tests 
//
//--> --> rootDir/projectName/project.h + project.cc, rootDir/tests/test_projectName.cc
//
//--> --> --> rootDir/projectName/tests/test_base.cc
int  AutoMakeSolution::run() {
	init();

	for(auto i : m_initDirs) {
		m_fileTool->createDir(i.c_str());
	}

	for(auto i : m_initFiles) {
		m_fileTool->createFile(i.c_str());
	}

	for(auto i : m_projectsName) {
		std::cout << "Solution_" << i << "____" << std::endl;
		m_projectTool->run(i);
	}

	return 0;
}



