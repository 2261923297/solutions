#pragma once
#include <iostream> 
#include <memory>
#include <stdio.h>

#include <map>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>
#include <string.h>

#define ENUM_CLASS_NAME LogLevel
#define ENUM_NAME Level

#define ENUM_BODY \
	ENUM_MESSAGE(Level, FATAL, ERROR, WARN_, INFO_, DEBUG)

// map<str, enum> 
#define ENUM_VALUES \
	xx(LogLevel::Level::FATAL) \
	xx(LogLevel::Level::ERROR) \
	xx(LogLevel::Level::WARN_)  \
	xx(LogLevel::Level::INFO_)  \
	xx(LogLevel::Level::DEBUG) \
//
#include "EnumReflection.h"



static size_t find_char(const char* chs, size_t size
	, char ch, std::vector<size_t>& vecs) {

	size_t rt = 0;
	for (size_t i = 0; i < size; i++) {
		if (chs[i] == ch) {
			vecs.push_back(i);
			rt++;
		}
	}
	return rt;
}

static std::string sp_file(const char* file_name) {
	std::vector<size_t> sps;
	size_t nsp = find_char(file_name, strlen(file_name), '/', sps);
	nsp = nsp > 2 ? 2 : nsp;
	std::string rt(file_name);
	return rt.substr(sps[sps.size() - nsp] + 1);
}

#define LOG_LEVEL(logger, level) 	\
	tt::system::EventVar(logger		\
	, tt::system::Event::ptr(		\
		new tt::system::Event(		\
			logger->getName()		\
			, level					\
			, __LINE__				\
			, sp_file(__FILE__)		\
			, __FUNCTION__			\
		))).getSS()					\
//

#define LOG_DEBUG(logger) LOG_LEVEL(logger, LogLevel::Level::DEBUG)
#define LOG_ERROR(logger) LOG_LEVEL(logger, LogLevel::Level::ERROR)
#define LOG_INFO(logger)  LOG_LEVEL(logger, LogLevel::Level::INFO_)

namespace tt {
namespace system {


class Event {
public:
	typedef std::shared_ptr<Event> ptr;

	Event(const std::string& loggerName, LogLevel::Level level
			, uint64_t line, const std::string& fileName
			, const std::string& func_name
		);

	~Event();
	
	const std::string& getFuncName() const { return m_func_name; }
	LogLevel::Level 
	getLevel() { return m_level; }

	std::stringstream& 
	getSS() { return m_ss; }
	
	uint64_t 
	getLine() const { return m_line; }
	
	const std::string&
	getFileName() const { return m_file; }
	
	const std::string
	getTab() const { return "\t";	}

	const std::string
	getNewLine() const { return "\n"; }

	const std::string
	getMessage() const;
	void 
	debugOut();

	const std::string&
	getLoggerName() const { return m_loggerName; }

	const std::string 
	getSpace() const { return " "; }

private:
	std::string m_loggerName;
	LogLevel::Level m_level;
	uint64_t    m_line;
	std::string m_file;
	std::string m_func_name;
	
	time_t      m_time;
	std::stringstream m_ss;
};

class FormatItem {
public: 
	typedef std::shared_ptr<FormatItem> ptr;
	FormatItem(const std::string& pattern) : m_pattern(pattern) { }
	virtual ~FormatItem() { }

	virtual void
	format(Event::ptr e, std::stringstream& ss) = 0;
	
protected:
	std::string m_pattern;
};

#undef xx
#define xx(C, EventFun)									\
class C : public FormatItem {							\
public:													\
	C(const std::string& pattern) : FormatItem(pattern) { }	\
	void										\
	format(Event::ptr e, std::stringstream& ss) override  \
	{													\
		ss << e->EventFun();							\
	}													\
};														\

//
class StringFormatItem : public FormatItem {
public: 
	StringFormatItem(const std::string& pattern) : FormatItem(pattern) { }

	void
	format(Event::ptr e, std::stringstream& ss) override {
		ss << m_pattern;
	}
};

xx(LevelFormatItem, getLevel)
xx(LineFormatItem, getLine)
xx(NewLineFormatItem, getNewLine)
xx(MessageFormatItem, getMessage)
xx(TabFormatItem, getTab)
xx(FileNameFormatItem, getFileName)
xx(LoggerNameFormatItem, getLoggerName)
xx(FunctionNameFormatItem, getFuncName)
xx(SpaceFormatItem, getSpace)
#undef xx
// #undef xx  //xx(C, EventFun)
// 

class Formatter {
public:
	typedef std::shared_ptr<Formatter> ptr;
	
	Formatter(const std::string& format) 
		: m_format(format){

		initFormatItem();
	}


	~Formatter() { 	}
 
	
	const std::string& 
	getFormat() const { return m_format; }

	void 
	setFormat(const std::string& format) { m_format = format; }

	std::string generate(Event::ptr e);
	void initFormatItem(); 
	void resetFormat(const std::string& format);

protected:

	std::string m_format;
	std::vector<FormatItem::ptr> m_items;
	char m_split = '/';
};

class Appandar {
public:
	typedef std::shared_ptr<Appandar> ptr;
	
	Appandar(LogLevel::Level level = LogLevel::Level::DEBUG) : m_level(level) { }
	virtual ~Appandar() { }
	
	LogLevel::Level 
	getLevel() { return m_level; }

	void 
	setLevel(const LogLevel::Level level) { m_level = level; }

	void 
	operator<<(const std::string& str) { this->appand(str); }

	virtual void
	appand(const std::string& str) = 0;
	
protected:
	LogLevel::Level m_level;
};

class StdoutAppandar : public Appandar{
public: 
	using ptr = std::shared_ptr<StdoutAppandar>;
	StdoutAppandar(LogLevel::Level level = LogLevel::Level::DEBUG) : Appandar(level) { }
	~StdoutAppandar() {	}

	void 
	appand(const std::string& str) override 
	{ std::cout << str << std::endl; }
};

class FileAppandar : public Appandar {
public:
	using ptr = std::shared_ptr<FileAppandar>;
	FileAppandar(const std::string& fileName) 
		: m_fileName(fileName) { }
protected:

	void 
	appand(const std::string& str) override {
		
	}

protected:
	std::string m_fileName;
};

class Logger {
public:
	typedef std::shared_ptr<Logger> ptr;

	Logger(const std::string& name, const std::string& format = 
		"/s{[}/l/s{]}/sp"
		"/s{[}/lm/s{]}/sp"
		"/s{>}/sp/m/tab"
		"/ln/s{:}/func/s{::}/fn"
	)  : m_name(name) {
		m_def_formatter = Formatter::ptr(new Formatter(format));
		m_def_appandar = Appandar::ptr(new StdoutAppandar);
	}

	~Logger() { }

	void
	setName(const std::string& name) { m_name = name; }

	const std::string& 
	getName() { return m_name; }

	void setFormatter(const std::string& str) 
	{ m_def_formatter = Formatter::ptr(new Formatter(str)); }

	void 
	setFormatter(Formatter::ptr formatter) { 
		m_def_formatter.reset();
		m_def_formatter = formatter;
	}


	void 
	setFormatter(Formatter* pFormatter) { m_def_formatter.reset(pFormatter); }

	void 
	addAppandar(Appandar::ptr appandar) { m_appandars.push_back(appandar);	}
	
	void 
	log(Event::ptr e);
protected:
	std::string m_name;
	std::vector<Appandar::ptr> m_appandars;
	Appandar::ptr  m_def_appandar;
	Formatter::ptr m_def_formatter;
};


class EventVar {
public:
	EventVar(Logger::ptr logger, Event::ptr e) 
		: m_event(e), m_logger(logger) { }
	~EventVar() { 
		m_logger->log(m_event);
	}

	std::stringstream& getSS() { return m_event->getSS(); }

private:
	Logger::ptr m_logger;
	Event::ptr m_event;
};  

} // namespace system    
} // namespace tt


static tt::system::Appandar::ptr
operator<<(tt::system::Appandar::ptr ap, const std::string& str) {
	ap->appand(str);
	return ap;
}

static tt::system::Logger::ptr 
	logger_system = tt::system::Logger::ptr(new tt::system::Logger("SYSTEM"));
static tt::system::Logger::ptr 
	logger_root = tt::system::Logger::ptr(new tt::system::Logger  ("ROOT__"));

#define DEBUG_SYS LOG_DEBUG(logger_system)
#define ERROR_SYS LOG_ERROR(logger_system)
#define INFO_SYS LOG_INFO(logger_system)

#define TT_DEBUG LOG_DEBUG(logger_root)
