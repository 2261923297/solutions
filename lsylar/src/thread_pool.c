#include "thread_pool.h"


void thread_pool::add_func(std::function<void*(void*)> cb, void* args) {
	m_tasks.insert(cb, args);

}


void thread_pool::run() {

	
}  
