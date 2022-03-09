#include "thread_pool.h"


namespace tt {
namespace system {


thread_pool::thread_pool(uint64_t thread_num ) {
	m_thread_num = thread_num;
	m_tasks.clear();
	m_sema = new std::counting_semaphore<100>(0);
	this->start();
}

thread_pool::~thread_pool() {
	this->stop();
}

void thread_pool::add_task(const task_t& t) {
	std::unique_lock<std::mutex> mtx;
	m_tasks.push_back(t);
	m_sema->relase();

}

void thread_pool::start() {
	for(int i = 0; i < m_thread_num; i++) {
		m_threads.push_back(std::bind(&thread_pool::run, this));
	}
}

void thread_pool::run() {
	while(1) {
		task_t current_task;
		if(m_tasks.size() != 0) {
			{
				std::unique_lock<std::mutex> mtx(m_mutex);
				current_task = m_tasks.front();
				m_tasks.pop_front();
			}
			current_task.cb();
		} else {
			m_sema->acquire	();
		}
	}
}
void thread_pool::stop() {

	for(int i = 0; i < m_thread_num; i++) {
		m_threads[i].join();
	}
}

} // thread
} // tt 
