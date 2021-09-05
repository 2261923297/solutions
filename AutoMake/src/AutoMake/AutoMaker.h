#include <memory>
#include <string>
#include "AutoMakeSolution.h"			//provide dir struct
#include "AutoMakeProject.h"			//provide file 
#include "AutoMakeSrc.h"				//provide file content


class AutoMaker {
public:
	typedef std::shared_ptr<AutoMaker> ptr;

	AutoMaker();
	~AutoMaker();

	virtual int initSolution();

	virtual int initProject();

	virtual int initSrc();

	virtual int run();

private:
	AutoMakeSolution::ptr m_solution;
	AutoMakeProject::ptr  m_project;
	AutoMakeSrc::ptr      m_src;


}; //AutoMaker




