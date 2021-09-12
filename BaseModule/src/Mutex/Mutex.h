#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <semaphore.h>

namespace tt {
class Mutex{ 
public: 
	using ptr = std::shared_ptr<Mutex>;
	Mutex();
	~Mutex();


protected:

}; // Mutex

class Semaphore {
public:
	using ptr = std::shared_ptr<Semaphore>;

	Semaphore(int value = 0, int pshared = 0) { init(value, pshared); }
	~Semaphore() { }

	void // value, semaphore num, pshared: is shared in process
	init(int value = 0, int pshared = 0) { sem_init(&m_sem, pshared, value); }

	void
	wait() { sem_wait(&m_sem); }

	void
	post(int num = 1) { while(num--) { sem_post(&m_sem); } }


protected:
	sem_t m_sem;

}; // Semaphore

}
