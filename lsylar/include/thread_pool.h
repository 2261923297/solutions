#pragma once

#include <unistd.h>
#include <semaphore>
#include <pthread.h>
#include <functional>
#include <map>
#include <stdint.h>
#include <memory>


#define MAX_TASK_NUM 100 * 10000
namespace tt {
namespace system {
	

class thread_pool {
public:
	using task_t = std::function<void*(void*)>;
	thread_pool(uint64_t thread_num = 5);
	~thread_pool();

	void add_func(task_t cb, void* args);
	void start();
	void stop();
	
	void run();  

private:
	std::vector<pthread_t> m_threads;
	std::map<std::function<task_t, void*> m_tasks;
	uint64_t m_thread_num;
	std::shared_ptr<std::counting_semaphore<MAX_TASK_NUM> > m_sema;
}; // class

} // namespace system
} // namespace tt

