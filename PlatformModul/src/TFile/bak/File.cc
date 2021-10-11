#include "File.h"

using namespace tt::File;

const std::string
Path::getcwd()  {
	char* buf = get_current_dir_name();
	std::string ans(buf);
	if(buf != nullptr) { free(buf); }
	return ans;
}

void
Path::init() {
	m_current_dir = getcwd();
	if(isAbs()) {
		m_abs_path = m_ori_path;
	} else {
		m_abs_path = getcwd();
		std::vector<std::string> abs_dirs;
		getdirs(abs_dirs, m_abs_path);
		std::vector<std::string> ori_dirs;
		getdirs(ori_dirs, m_ori_path);
		
		for(auto dirname : ori_dirs) {
			if(dirname == std::string("."))
				continue;
			else if(dirname == std::string("..")) {
				abs_dirs.pop_back();
			} else {
				abs_dirs.push_back(dirname);
			}
		}
		m_abs_path = "";
		std::stringstream ss;
		for(auto dirname : abs_dirs) {
			ss << "/" << dirname;
		}
		m_abs_path = ss.str();
	}
		
	getdirs(m_dirs, m_abs_path);
	m_name = m_dirs[m_dirs.size() - 1];
}


void 
Path::getdirs(std::vector<std::string>& dirs, const std::string& path)  {
	std::vector<size_t> split_ins;
	std::string dirname;
	
	size_t name_beg = 0;
	if(path[0] != m_dir_split) {
		name_beg = 0;
	} else {
		name_beg = 1;
	}
	
	for(int i = 1; i < path.size(); i++) {
		if(path[i] == m_dir_split) 
			split_ins.push_back(i);
	}
	if(path[path.size() - 1] != m_dir_split)
		split_ins.push_back(path.size());


	for(auto name_end : split_ins) {
		dirname = path.substr(name_beg, name_end - name_beg);
		name_beg = name_end + 1;
		dirs.push_back(dirname);
	}
}

void 
Path::reset(const std::string& path) { 
	if(0 != path.size())
		m_ori_path = path; 	
	m_dirs.clear();
	init();
}
void 
Path::showData() const {
	std::stringstream ss;
	ss  << "Path"
		<< "\n\tsplit:" << m_dir_split
		<< "\n\tpath: " << m_ori_path
		<< "\n\tabs:  "	<< m_abs_path
		<< "\n\tcur_dir: " << m_current_dir
		<< "\n\tname: " << m_name
		<< "\n\tdirs: ";
	for(auto dirname : m_dirs) {
		ss << dirname << ", ";
	}
	ss << std::endl;
	std::string ans = ss.str(); 
	std::cout << ans;
}

void 
Data::reset(const std::string& path) { 
	if(0 != path.size())
		m_file_path = path; 
	int ans = stat(path.c_str(), &m_data);
	m_exit = !ans;
}

void 
Data::showData() {
	std::stringstream ss;
	
	ss  << "Data: "
		<< "\n\tpath: " << getPath()
		<< "\n\texit: "	<< exit()
		<< "\n\tisDir: " << isDir()
		<< "\n\tisReg: " << isReg()
		<< "\n\tsize: "  << size()
		<< std::endl;
	std::string ans = ss.str();
	std::cout << ans;

}
int
Entry::reopen(const std::string& mode) {
	if(m_stream) 
		fclose(m_stream);
	m_stream = nullptr;

	m_stream = fopen(m_path->path().c_str(), mode.c_str());
	
	if(m_stream == nullptr) 
		perror("can^t fopen: ");
	return nullptr != m_stream;
}

int 
Entry::write(const char* buffer, size_t size, uint64_t pos) {
	if(pos != 0) {
		setPos(pos);
	}

	int ans = fwrite(buffer, 1, size, m_stream);
	if(ans <= 0) {
		perror("can^t fwrite: ");
	}
	return ans;
}

int 
Entry::read(char* buffer, size_t size, uint64_t pos) {
	if(pos != 0) {
		setPos(pos);
	}
	int ans = fread(buffer, 1, size, m_stream);
	if(ans <= 0) {
		perror("can^t fread: ");
	}
	return ans;
}

uint64_t
Entry::getPos() { 
	int ans = fgetpos(m_stream, &m_pos);
	if(ans) {
		perror("Entry::getPos->fgetpos: ");
	}
	return m_pos.__pos; 
}

void 
Entry::setPos(uint64_t pos) { 
	fpos_t fpos;
	fpos.__pos = pos;
	int ans = fsetpos(m_stream, &fpos);
	if(ans) {
		perror("Entry::setPos->fsetpos: ");
	}
	getPos();
}


void
Entry::init(const std::string& path) {
	m_stream = nullptr;  //初始值为非0
	m_pos = { 0 };		 //初始值为非0
	resetPath(path);
	m_data = Data::ptr(new Data(m_path->path()));
}


int
LinuxDirMaker::mkdir(const std::string& path, mode_t mode) {
//	std::cout << "LinuxDirMaker->mkdir: " << path << std::endl;
	int ans = ::mkdir(path.c_str(), mode);
	if(ans == -1) {
		perror("[error] > LinuxDirMaker::mkdir");
	}
	return ans;
}





















