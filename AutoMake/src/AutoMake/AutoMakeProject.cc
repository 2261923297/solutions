#include "AutoMakeProject.h" 

AutoMakeProject::AutoMakeProject(const std::string& rootPath, const std::string& projectName) {
	setRootPath(rootPath);
	setProjectName(projectName);

	m_fileTool = TFile::ptr(new TWinFile(m_rootPath.c_str()));
	
	m_fileTool->formateDirName(m_rootPath);
}

AutoMakeProject::~AutoMakeProject() {
	std::cout << "rootProject: " << m_rootPath << " is end!" << std::endl;
}

int AutoMakeProject::initProjectFile() {
	std::string headFile = getHeadPath();
	std::string srcFile = getSrcPath();

	std::string mkFile = getMkPath();
	std::string tBase = getTestPath();

	m_files.clear();
	m_files.push_back(tBase);
	m_files.push_back(headFile);
	m_files.push_back(srcFile);
	m_files.push_back(mkFile);

	return 4;
}


std::string AutoMakeProject::init() {
	std::string ans = "";
	std::string projectPath = m_rootPath + m_projectName;
	m_fileTool->formateDirName(projectPath);

	ans = projectPath;
	std::cout << "init project_path: " << projectPath << std::endl;
	
	initProjectFile();

	return ans;
}

int AutoMakeProject::run(const std::string& projectName) {
	if(projectName.size()) {
		setProjectName(projectName);
	}
	std::string projectPath = init();
	if(!projectPath.size()) {
		std::cout << "init projectName err " << __FILE__ << ": " << __LINE__ << std::endl;
		return -1;
	}
	
	m_fileTool->createDir(projectPath.c_str());
	
	for(auto i : m_files) {
		m_fileTool->createFile((projectPath + i).c_str());
	}

	return 0;
}
