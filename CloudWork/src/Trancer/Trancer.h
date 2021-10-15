#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "TNet.h"
#include "File.h"

namespace tt { 
class Task {
public:
	using ptr = std::shared_ptr<Task>;
	Task(const std::string& file_name, task_type type) { reset(file_name, type); } 
	~Task() { }
	enum task_type { SEND, RECV };

	const std::string& file() const { return m_file; }
	void set_file(const std::string& file) { m_file = file; }

	const task_type& type() const { return m_type; }
	void set_type(const task_type& type) { m_type = type; }

	void reset(const std::string& file_name, task_type type) { 
		set_file(file_name);
		set_type(type);
	}

private:
	std::string m_file;
	task_type   m_type;

}; // class Task

class Trancer{ 
public: 
	using ptr = std::shared_ptr<Trancer>;
	Trancer() { }
	~Trancer() { }
	
	virtual void run(tt::net::Socket::ptr sock, const std::string& file_name) = 0;
	
protected:

}; // Trancer

class SendTrancer : public Trancer {
public:
	SendTrancer
private:
	tt::net::Socket::ptr m_sock;
	tt::File::Entry::ptr m_file;
}; // class SendTrancer

class TrancerManager {
public:


private:
		
}

} // namespcae tt 


