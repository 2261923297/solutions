#include "Thread.h"
#include "Log.h"

#include "util.h"
#include <string>

#ifndef handle_error_en
#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); return EXIT_FAILURE; } while(0)
#endif

#ifndef handle_error
#define hansle_error(msg) \
	do { perror(msg); retrun EXIT_FAILURE; } while(0)
#endif

namespace tt {
namespace system {

thread_local std::string t_thread_name = "UNKNOWN";
thread_local tt::system::Thread* t_thread = nullptr;

tt::system::Thread*
tt::system::Thread::GetThis() { return t_thread; }

const std::string&
tt::system::Thread::GetName() { return t_thread_name; }

void
tt::system::Thread::SetName(const std::string& thread_name) { t_thread_name = thread_name; }

/*
 * 执行顺序
 *
 * pthread_create -> Thread::run -> cb -> ~Thread(), wait cb;
 * */

tt::system::Thread::Thread(std::function<void()> cb, const std::string& name) {
	m_cb = cb;
	m_name = name;

	pthread_attr_t p_attr;
	pthread_attr_init(&p_attr);
	int rt = pthread_create(&m_thread, &p_attr, &Thread::run, this);
	std::cout << "after pthread_ceate" << std::endl;

	m_sem.wait();

	if(rt != 0) {
		std::cout << "pthread_create fail, name = " << m_name << "\n";
	}
} 

tt::system::Thread::~Thread() {
	m_sem.wait();    //函数没有执行完毕， 不能析构
	/*
	if(m_thread) {
		pthread_detach(m_thread);
	} 
	*/
}

void Thread::join() {
	if(m_thread) {
		int rt = pthread_join(m_thread, nullptr);
		if(rt) {
			TT_DEBUG << "pthread_join error, thread_name = " << m_name;
			throw std::logic_error("pthread_join error");
		}
		m_thread = 0;
	}
}
    
void* 
tt::system::Thread::run(void* thread_msg) {
	Thread* thread = (Thread*)thread_msg;

	t_thread = thread;
	t_thread_name = thread->m_name;
	thread->m_id = tt::system::GetThreadId();
	pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());

	std::function<void()> cb;
	cb.swap(thread->m_cb);
	thread->m_sem.post();

	cb();

	thread->m_sem.post();   
	return nullptr;
}

    
const std::string
tt::system::Thread::to_string() const {
	std::string ans;
	std::stringstream ss;
	ss << "tid: " << m_id << "\n"
		<< "name: " << m_name;
	

	ans = ss.str();
	return ans;
}

} // namespace system
} // namespace tt

