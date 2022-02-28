#pragma once

#include <pthread.h>

#include <iostream>
#include <memory>
#include <string>
#include <semaphore.h>

namespace tt {
namespace system {

template<class T>
struct ScopedLockImpl {
public:
	ScopedLockImpl(T& mutex) : m_mutex(mutex) { m_mutex.lock(); m_locked = true; }
	~ScopedLockImpl() { unlock(); }

	void lock() {
		if(!m_locked) {
			m_mutex.lock();
			m_locked = true;
		}
	}

	void unlock() {
		if(m_locked) {
			m_mutex.unlock();
			m_locked = false;
		}
	}
private:
	T&	 m_mutex;
	bool m_locked;
}; // struct ScopedLockImpl

class Mutex { // pthread_mutex_t 
public: 
	using ptr = std::shared_ptr<Mutex>;
	using Lock = ScopedLockImpl<Mutex>;
	Mutex();
	~Mutex();

	void lock() { pthread_mutex_lock(&m_mutex); }
	void unlock() { pthread_mutex_unlock(&m_mutex); }

protected:
	pthread_mutex_t m_mutex;
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

} // namespace system
} // namespace tt 
