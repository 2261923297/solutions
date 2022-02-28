#include <iostream>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <memory>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>


#define ENUM_CLASS_NAME Platform
#define ENUM_NAME Type
#define ENUM_BODY \
	ENUM_MESSAGE(Type, WIN, LINUX, UNKNOWN) \
//	
#define ENUM_VALUES \
	xx(Platform::Type::WIN) \
	xx(Platform::Type::LINUX) \
	xx(Platform::Type::UNKNOWN) \
//

#include "../../include/EnumReflection.h"
/*
#define ENUM_CLASS_NAME FileType
#define ENUM_NAME Type
#define ENUM_BODY \
	ENUM_MESSAGE(Type, )
#define ENUM_VALUES \
	xx(FileType::Type::)
*/
#ifndef PLATFORM
#define PLATFORM Platform::Type::LINUX
#endif
namespace tt {
namespace File {

class Data {
public:
	using data_t = struct stat;
	using ptr = std::shared_ptr<Data>;

	Data(const std::string& path) { reset(path); }
	
	virtual ~Data() { }

	void reset(const std::string& path = "");
	const data_t& getData() { return m_data; }
	const std::string& getPath() { return m_file_path; }

	int exit() { return m_exit; }
	int isDir() { return S_ISDIR(m_data.st_mode); }
	int isReg() { return S_ISREG(m_data.st_mode); }
	size_t size() { return m_data.st_size; }

	void 
	showData();
private:
	std::string   m_file_path;
	data_t        m_data;			
	int           m_exit;
}; // Data

class Path {
public:
	using ptr = std::shared_ptr<tt::File::Path>;

	Path(const std::string& path, char dir_split = '/') 
	: m_dir_split(dir_split) { reset(path); }
	~Path() { }
	void reset(const std::string& path = ""); 

	void getdirs(std::vector<std::string>& dirs, const std::string& path);
	void setSplit(const char val) { m_dir_split = val; }

	const std::string& absolute() const { return m_abs_path; }
	const char& split() const { return m_dir_split; }

	const std::string&	current() const { return m_current_dir; }
	const std::string& 	path() const { return m_ori_path; }
	const std::string&	name() const { return m_name; }
	const std::vector<std::string>& dirs() const { return m_dirs; }
	
	virtual int isAbs() const { return m_ori_path[0] == '/'; }

	void showData() const;
protected:
	virtual const std::string getcwd();

	void init();
protected:
	char		m_dir_split;
	std::string m_ori_path;
	std::string m_abs_path;
	std::string m_current_dir;

	std::vector<std::string> m_dirs;
	std::string m_name;
}; //Path

class Entry {
public:

	typedef std::shared_ptr<tt::File::Entry> ptr;

	Entry(std::string& path) { init(path);  }
	Entry(const Path& path) { init(path.path()); }
	virtual ~Entry() { }
		
	void resetPath(const std::string& path) { m_path.reset(new Path(path)); }
	void resetData(const std::string& path = "") { m_data->reset(path); }
	
	const Path::ptr getPath() { return m_path; }
	const Data::ptr getData() { return m_data; }

	int reopen(const std::string& mode = "a+");
	void close() { if(m_stream) { fclose(m_stream); m_stream = nullptr;	} }

	int write(const char* buffer, size_t data, uint64_t pos = 0);
	int read(char* buffer, size_t size, uint64_t pos = 0);

	uint64_t getPos();
	void setPos(uint64_t pos);


protected:

	void init(const std::string& path); 
protected: 
	FILE*     m_stream;
	fpos_t    m_pos;
	Path::ptr m_path;
	Data::ptr m_data;
}; // Entry

class LinuxDirMaker {
public:
	static int 
	mkdir(const std::string& path, mode_t mode);
	
}; // LinuxDirMaker

template<class MkdirClass = LinuxDirMaker>
class FileManager {
public:
	using ptr = std::shared_ptr<FileManager>;
	using DirMaker = MkdirClass;

	FileManager() { }
	virtual ~FileManager() { }

	static int	mkdir(const std::string& dirName, mode_t mode = 0700) { return DirMaker::mkdir(dirName, mode); }
	size_t getDirSize(const std::string& dirName) { return 0; }

	static void
	mkdirs(const std::string& dirs) {
		Path::ptr p(new Path(dirs));
		Data::ptr d(new Data(p->absolute()));
		const std::vector<std::string> &p_dirs = p->dirs();
		std::stringstream dir_ss;
		auto dir_iter = p_dirs.begin();
		for(; dir_iter != p_dirs.end(); dir_iter++) {
			dir_ss << p->split() << *dir_iter;
			d->reset(dir_ss.str());
			if(d->exit() && d->isDir()) {
				continue;
			}
			std::cout << "dir: " << dir_ss.str() 
					<< " is not exit!" << std::endl;
			break;
		}
		if(-1 == mkdir(dir_ss.str())) {
			return;
		}
		dir_iter++;
		for(; dir_iter != p_dirs.end(); dir_iter++) {
			dir_ss << p->split() << *dir_iter;
			if(-1 == mkdir(dir_ss.str())) {
				break;
			}
		}
	}
protected:

}; // Filanager

} // namespace File


}
