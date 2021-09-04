#pragma once
#include <iostream> 
#include <memory>

#include <map>
#include <vector>
#include <stdint.h>
#include <functional>
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



#define LOG_LEVEL(level) \
	ttlog::EventVar(ttlog::Event::ptr(new ttlog::Event(level, __LINE__, __FILE__))).getSS()

#define LOG_DEBUG LOG_LEVEL(LogLevel::Level::DEBUG)

namespace ttlog {

class Event {
public:
	typedef std::shared_ptr<Event> ptr;

	Event(
		  LogLevel::Level level
		, uint64_t line
		, std::string fileName

			): m_line(line), m_file(fileName), m_level(level) {

	}

	~Event() {

	}
	
	LogLevel::Level 
	getLecel() { return m_level; }

	std::stringstream& 
	getSS() { return m_ss; }
	
	uint64_t 
	getLine() { return m_line; }
	
	std::string 
	getFileName() { return m_file; }
	
	void 
	debugOut() {
		std::cout << "[" << m_level << "] "
			<< m_file << ": " << m_line
			<< "\t" << m_ss.str() << std::endl;
	}

private:
	LogLevel::Level m_level;
	uint64_t    m_line;
	std::string m_file;
	
	time_t      m_time;
	std::stringstream m_ss;
};

class FormatItem {
public: 
	std::shared_ptr<FormatItem> ptr;
	FormatItem(std::string pattern = "") : m_pattern(pattern) { }
	virtual ~FormatItem();
	
	void 
	setPattern(const std::string& val) { m_pattern = val; }

	const std::string& 
	getPattern() {	return m_pattern; }

	virtual int 
	format(Event::ptr e, std::stringstream& ss) = 0;

protected:
	std::string m_pattern;
};

#undef xx
#define xx(C, EventFun)									\
class C : public FormatItem {							\
public:													\
	C(std::string pattern = "") : FormatItem(pattern) { }	\
	int													\
	format(Event::ptr e, std::stringstream& ss) override  \
	{													\
		ss << e->EventFun();							\
	}													\
};														\
//
xx(LevelFormatItem, getLevel)


#undef xx  //xx(C, EventFun)

class Formatter {
public:
	typedef std::shared_ptr<Formatter> ptr;
	
	Formatter(const std::string& format, LogLevel::Level level) 
		: m_format(format), m_level(level) { }

	~Formatter() { 	}

	void initFormatItem();


	void resetFormat(const std::string& format) {
		m_item.clear();
		setFormat(format);
		initFormatItem();
	}
	
	const std::string& getFormat() const { return m_format; }

	void setFormat(const std::string& format) { m_format = format; }

	void setLevel(LogLevel::Level level) { m_level = level;	}

	const LogLevel::Level getLevel() const { return m_level; }

	std::string generate(Event::ptr e) {
		std::stringstream ss;
		for(auto item : m_items) {
			item->format(e, ss);
		}
		return ss.str();
	}

public:

protected:
	LogLevel::Level m_level;
	std::string m_format;
	std::vector<FormatItem::ptr> m_items;
}

class Appandar {
public:
	typedef std::shared_ptr<Appandar> ptr;

	
protected:

}

class Logger {
public:
	std::shared_ptr<Logger> ptr;

	Logger(const std::string& name);

protected:
	std::string m_name;
	std::vector<Appandar::ptr> m_appandars;
	Formatter::ptr m_def_formatter;
};


class EventVar {
public:
	EventVar(Event::ptr e) : m_event(e) { }
	~EventVar() { m_event->debugOut(); }

	std::stringstream& getSS() { return m_event->getSS(); }

private:
	Event::ptr m_event;
};  



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

} // namespace ttlog    


