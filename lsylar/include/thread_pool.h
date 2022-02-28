#pragma once

#include <unistd.h>
#include <semaphore>
#include <pthread.h>
#include <functional>

class thread_pool {
public:
	thread_pool();
	~thread_pool();


	void add_func(std::function<void*(void*)> cb, void* args);
	void run();  

private:

	std::vector<pthread_t> m_pools;
	std::map<std::function<void*(void*)>, void* args> m_tasks

}; // class


