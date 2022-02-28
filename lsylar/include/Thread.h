#pragma once

#include <iostream> 
#include <memory>
#include <pthread.h>
#include <sstream>
#include <functional>
#include <unistd.h>

#include "util.h"
#include "Semaphore.h"
namespace tt {
namespace system {


class Thread {
public: 
    typedef std::shared_ptr<Thread> ptr; 
    
	Thread(std::function<void()> cb, const std::string& name = "UNKNOWN");
    virtual ~Thread();
    
    const std::string
	to_string() const;

	const std::string&
	getName() const { return m_name; }
	void 
	setName(const std::string& name) { m_name = name; };

	const pid_t& 
	getId() const { return m_id; }
	const pthread_t&
	getPthread() const { return m_thread; }

	void join();
public:
	static const std::string& 
	GetName();

	static Thread*
	GetThis();

	static void
	SetName(const std::string& thread_name);

protected:
private:
	Thread(Thread&);
	Thread(Thread&&);
	Thread& operator=(Thread&);
	Thread& operator=(Thread&&);	
	static void*
	run(void* thread_msg);
private: 
	Semaphore m_sem;

	pid_t m_id;
	pthread_t m_thread;
	std::function<void()> m_cb;
	std::string m_name;

}; // Thread

} // namespace system
} // namespace tt    
