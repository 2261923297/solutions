#pragma once 

#include "Log.h"
#include "Mutex.h"
#include "Thread.h"
#include "Fiber.h"

#include <atomic>
#include <list>
#include <memory>
#include <string>

namespace tt {
namespace system {

class scheduler {
public:
	using MutexType = Mutex; 
	scheduler(size_t nThreads = 1, bool use_caller = true, const std::string& name = "def_scheduler");
	virtual ~scheduler();



	const std::string& name() const { return m_name; }
	static scheduler* GetThis();
	static Fiber* GetMainFiber();

	void start();
	void stop();

	template<class FiberOrCb>
	void schedule(FiberOrCb fc, int thread = -1) {
		bool need_tickle = false;
		{
			MutexType::Lock lock(m_mutex);
			need_tickle = scheduleNoLock(fc, thread);
		}
		if(need_tickle) {
			tickle();
		}

	}

	template <class InputIterator>
	void schedule(InputIterator begin, InputIterator end) {
		bool need_tickle = false;
		{
			MutexType::Lock lock(m_mutex);
			while(begin != end) {
				need_tickle = scheduleNoLock(&*begin, -1) || need_tickle;
				++begin;
			}
		}
		if(need_tickle) {
			tickle();
		}
	}
protected:

	virtual void tickle();  // notice task
	void run();   // 协程调度
	virtual bool stopping();  // 是否可以停止
	virtual void idle();	  // 无task时idle
	void set_this();		  // 设置当前协程调度器
	bool has_idle_threads() { return m_idle_thread_count > 0; }

private:
	template<class FiberOrCb>
	bool scheduleNoLock(FiberOrCb fc, int thread) {
		bool need_tickle = m_fibers.empty();
		FiberAndThread ft(fc, thread);
		if(ft.fiber || ft.cb) {
			m_fibers.push_back(ft);
		}
		return need_tickle;
	}

	struct FiberAndThread {
		Fiber::ptr fiber;
		std::function<void()> cb;
		int thread;					// 预计执行线程id

		FiberAndThread(Fiber::ptr f, int thr) 
			: fiber(f), thread(thr) {}
		FiberAndThread(Fiber::ptr* f, int thr) 
			: thread(thr) {
			fiber.swap(*f);
		}

		FiberAndThread(std::function<void()> f, int thr) 
			: cb(f), thread(thr) {}
		FiberAndThread(std::function<void()>* f, int thr) 
			: thread(thr) {
			cb.swap(*f);
		}
		FiberAndThread() 
			:thread(-1) {
			reset();
		}
		void reset() {
			fiber = nullptr;
			cb = nullptr;
			thread = -1;
		}
	};
private:
	MutexType  m_mutex;
	std::vector<Thread::ptr> m_thread_pool;   // thread pool
	std::list<FiberAndThread> m_fibers;   // task queue

	Fiber::ptr  m_root_fiber;
	std::string m_name;

protected:
	std::vector<int> m_thread_ids;
	size_t m_thread_count;
	std::atomic<size_t>  m_active_thread_count = { 0 };
	std::atomic<size_t>  m_idle_thread_count = { 0 };
	bool m_stopping = true;
	bool m_auto_stop = false;
	int  m_root_thread = 0;

}; // class scheduler


} // namespace system
} // namespace system


