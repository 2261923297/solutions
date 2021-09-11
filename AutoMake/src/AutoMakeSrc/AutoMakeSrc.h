#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <map>

class ContextMaker {
public:
	using ptr = std::shared_ptr<ContextMaker>;
	ContextMaker() { }
	virtual ~ContextMaker() { }
	
	const std::string
	getContext(); 

	virtual void
	makeContext() = 0;

	static const std::string&
	GetProjectName() { return s_projectName; }

	static const std::string&
	GetProjectPath() { return s_projectPath; }

	static const std::string&
	GetBinDir() { return s_binDir; }

	static void
	SetProjectName(const std::string& val) { s_projectName = val; }

	static void
	SetProjectPath(const std::string& val) { s_projectPath = val; }

	static void
	SetBinDir(const std::string& val) { s_binDir = val; }

	void showData();
public:
	static std::string s_projectName;
	static std::string s_projectPath; // to delete arg
	static std::string s_binDir;	

protected:
	std::stringstream m_ss;

}; // ContextMaker

#define xx(class_name) \
class class_name : public ContextMaker { \
public:										\
	void									\
	makeContext() override;					\
}; // class_name							\
//
xx(HeadContextMaker)
xx(SrcContextMaker)
xx(TestContextMaker)
xx(MakeContextMaker)
#undef xx

class AutoMakeSrc {
public:
	using ptr = std::shared_ptr<AutoMakeSrc>;
	AutoMakeSrc() { }
	virtual ~AutoMakeSrc() { }

	void 
	addMaker(const std::string& file_path, const ContextMaker::ptr& maker) 
	{ m_makers.insert( { file_path, maker} ); }

	void showStrings() {
		for(auto maker : m_makers) 
		  { std::cout << "[" << maker.first << "]" << ": \n" << maker.second->getContext() << std::endl; }
	}

	const std::string
	getContext(const std::string& context_desc) 
		{ return (m_makers[context_desc])->getContext(); }
	
	

protected:

	std::map<const std::string, ContextMaker::ptr> m_makers;
//	std::vector<ContextMaker::ptr> m_makers;

}; // AutoMakeSrc







