

#include <iostream>
#include <string>

#include "../../include/AutoMakeSlt.h"
#include "../../include/AutoMakeSrc.h"
#include "../../include/AutoMakePjt.h"
#include "../../include/File.h"

template<class DirMaker = AutoMakeSlt, class FileMaker = AutoMakePjt, class SrcMaker = AutoMakeSrc>
class AutoMaker {
public:
	using ptr = std::shared_ptr<AutoMaker>;
	using DM = DirMaker;
	using FM = FileMaker;
	using SM = SrcMaker;

	AutoMaker() { }
	virtual ~AutoMaker() { }

	void
	mkslt(const std::string& slt_name, const std::string& root_dir) {
		m_dm.reset(new DM);
		m_dm->setRootDir(root_dir);
		m_dm->setSltName(slt_name);
		
		std::string slt_dir = m_dm->getRootDir() + m_dm->getSltName() + '/';
		
		m_file_manager->mkdirs(slt_dir); 
		m_dm->addDir("bin");
		m_dm->addDir("doc");
		m_dm->addDir("src");
		m_dm->addDir("include");
		m_dm->addDir("lib");
		m_dm->addDir("test");
		m_dm->addDir("examples");
		
		const std::vector<std::string>& dirs = m_dm->getDirs();
		for(auto dir : dirs) {
			m_file_manager->mkdir(slt_dir + dir);
		}
	}

	void 
	mkpjt(const std::string& pjt_name, const std::string& pjt_dir) {
		std::string pjt_path = pjt_dir + pjt_name + '/';
		m_fm.reset(new FM);
		m_sm.reset(new SM);
			
		ContextMaker::SetProjectName(pjt_name);
		ContextMaker::SetProjectPath(pjt_dir);

		m_fm->setPjtName(pjt_name);
		m_fm->setPjtDir(pjt_dir);

		tt::File::Data pjt_dir_data(pjt_path);
		if(!(pjt_dir_data.exit() && pjt_dir_data.isDir())) {
			m_file_manager->mkdirs(pjt_path);
		}
		
		std::string header = pjt_name + ".h";
		std::string source = pjt_name + ".cc";
		std::string mk = "mk";
		std::string test = std::string("test_") + pjt_name + std::string("_func.cc");
		std::string context = "";
#undef xx
#define xx(file_name, C)							\
		m_fm->addFile(file_name);					\
		m_sm->addMaker(file_name, C::ptr(new C));	\
		context = m_sm->getContext(file_name);		\
		m_file_entry.reset(new tt::File::Entry(pjt_path + file_name)); \
		m_file_entry->reopen();						\
		m_file_entry->write(context.c_str(), context.size()); \
		m_file_entry->close();						\
//
		xx(header, HeadContextMaker)
		xx(source, SrcContextMaker)
		xx(test, TestContextMaker)
		xx(mk, MakeContextMaker)
#undef xx // xx(file_name, C)
	}
	
protected:
	typename DM::ptr m_dm;
	typename FM::ptr m_fm;
	typename SM::ptr m_sm;
	
	tt::File::Entry::ptr m_file_entry;
	typename tt::File::FileManager<tt::File::LinuxDirMaker>::ptr  m_file_manager;
	
}; // AutoMaker

#define AUTOMAKER AutoMaker<AutoMakeSlt, AutoMakePjt, AutoMakeSrc>

void 
mkpjt(const std::string& pjt_name, const std::string& pjt_dir = "./") {
	std::cout << "make project: " << pjt_name
			<< "\n     die    : " << pjt_dir 
			<< std::endl;
	AUTOMAKER::ptr am(new AUTOMAKER);
	am->mkpjt(pjt_name, pjt_dir);


}

void
mkslt(const std::string& slt_name, const std::string& root_dir = "./") {
	std::cout << "make solution: " << slt_name 
			<< "\n     dir     : " << root_dir
			<< std::endl;
	AUTOMAKER::ptr am(new AUTOMAKER);
	am->mkslt(slt_name, root_dir);
}

int
main(int argc, char** argv) {
	if(argc < 4) {
		std::cout << "[right use] command [-s][-p][-?] <dir> <name>" << std::endl;
		return 1;
	}
	std::string operation = argv[1];
	std::string dir = argv[2];
	std::string name = argv[3];
	if(operation == "-s") {
		mkslt(name, dir);
	} else if(operation == "-p") {
		mkpjt(name, dir);
	} else {
		std::cout << "[right use] command [-s][-p][-?] <dir> <name>" << std::endl;
		return 1;
	}
//	mkslt("slt", "./");
//	mkpjt("pjt", "./");
	return 0;
}
