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

#define ENUM_CLASS_NAME LogLevel
#define ENUM_NAME Level

#define ENUM_BODY \
	ENUM_MESSAGE(Level, FATAL, ERROR, WARN, INFO, DEBUG)

#define ENUM_VALUES \
	xx(LogLevel::Level::FATAL) \
	xx(LogLevel::Level::ERROR) \
	xx(LogLevel::Level::WARN)  \
	xx(LogLevel::Level::INFO)  \
	xx(LogLevel::Level::DEBUG) \
//
#include "../../include/EnumReflection.h"

#define LOG_LEVEL(logger, level) \
	ttlog::EventVar(logger, ttlog::Event::ptr(new ttlog::Event(logger->getName(), level, __LINE__, __FILE__))).getSS()

#define LOG_DEBUG(logger) LOG_LEVEL(logger, LogLevel::Level::DEBUG) \

namespace ttlog {

class Event {
public:
	typedef std::shared_ptr<Event> ptr;

	Event(const std::string& loggerName, LogLevel::Level level
			, uint64_t line, std::string fileName
		);

	~Event();
	
	LogLevel::Level 
	getLevel() { return m_level; }

	std::stringstream& 
	getSS() { return m_ss; }
	
	uint64_t 
	getLine() { return m_line; }
	
	std::string 
	getFileName() { return m_file; }
	
	std::string
	getTab() { return "\t";	}

	std::string 
	getNewLine() { return "\n"; }

	std::string
	getMessage() { return m_ss.str(); }
	void 
	debugOut();

	std::string& 
	getLoggerName() { return m_loggerName; }

private:
	std::string m_loggerName;
	LogLevel::Level m_level;
	uint64_t    m_line;
	std::string m_file;
	
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
/*
class LevelFormatItem : public FormatItem {
public:
	LevelFormatItem(const std::string& pattern) : FormatItem(pattern) { }

	void
	format(Event::ptr e, std::stringstream& ss) override {
		ss << e->getLevel();
	}
};
*/
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
public:

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

	Logger(const std::string& name, const std::string& format = "/s{[}/l/s{]}/tab/s{[}/lm/s{]}/tab/fn/s{::}/ln/s{:}/tab/m") : m_name(name) {
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


/*

class Log {
public: 
    typedef std::shared_ptr<Log> ptr; 
    
    Log(); 
    virtual ~Log();
	
	int log(Event::ptr logEvent) {
		logEvent->debugOut();

		return 0;
	}
    
private: 
	    
}; // Log
*/
} // namespace ttlog    

static void 
operator<<(ttlog::Appandar::ptr ap, const std::string& str) {
	ap->appand(str);
}
static ttlog::Logger::ptr logger_system = ttlog::Logger::ptr(new ttlog::Logger("SYSTEM"));
static ttlog::Logger::ptr logger_root = ttlog::Logger::ptr(new ttlog::Logger("ROOT"));

#define DEBUG_SYS LOG_DEBUG(logger_system)
#define TT_DEBUG LOG_DEBUG(logger_root)
