#include "scheduler.h"
#include "macro.h"

namespace tt {
namespace system {

static thread_local scheduler* t_scheduler = nullptr;
static thread_local Fiber* t_scheduler_fiber = nullptr;


scheduler::scheduler(size_t nThreads, bool use_caller, const std::string& name) 
	: m_name(name){
	TT_ASSERT(nThreads > 0);
	if(use_caller) {
		tt::system::Fiber::GetThis();
		--nThreads;

		// 不能有两个调度器
		TT_ASSERT(GetThis() == nullptr);
		t_scheduler = this;
		
		// root_fiber 绑定调度器run方法
		m_root_fiber.reset(new 
				tt::system::Fiber(std::bind(&scheduler::run, this)));
		tt::system::Thread::SetName(m_name);

		// 调度器协程是 root fiber
		t_scheduler_fiber = m_root_fiber.get();
		m_root_thread = tt::system::GetThreadId();   // user_caller 获取root thread


		m_thread_ids.push_back(m_root_thread); 
		
	} else {
		m_root_thread = -1;
	}
	
	m_thread_count = nThreads;						// 没有 root thread
}

scheduler::~scheduler() {
	TT_ASSERT(m_stopping);
	if(GetThis() == this) {
		t_scheduler = nullptr;
	}
}

scheduler* 
scheduler::GetThis() { return t_scheduler; }

Fiber* 
scheduler::GetMainFiber() {	return t_scheduler_fiber; }


/**
 *	1.线程池启动线程, 初始化线程
 *
 * */
void scheduler::start() {
	MutexType::Lock lock(m_mutex);
	if(! m_stopping) {
		return;
	}
	m_stopping = false;
	TT_ASSERT(m_thread_pool.empty());

	m_thread_pool.resize(m_thread_count);
	for(size_t i = 0; i < m_thread_pool.size(); i++) {
		m_thread_pool[i].reset(new Thread(std::bind(&scheduler::run, this)
					                      , std::to_string(i)));	
		m_thread_ids.push_back(m_thread_pool[i]->getId());
	}

}

void scheduler::stop() {
	m_auto_stop = true;
	if(m_root_fiber
			&& m_thread_count == 0
			&& (m_root_fiber->get_state() == Fiber::TERM
				|| m_root_fiber->get_state() == Fiber::INIT)) {
		TT_DEBUG << "scheduler: " << m_name << "stopped";
		m_stopping = true;
		if(stopping()) {
			return;
		} 
	}

	if(m_root_thread != -1) {
		TT_ASSERT(GetThis() == this);
	} else {
		TT_ASSERT(GetThis() != this);
	}

	m_stopping = true;
	for(size_t i = 0; i < m_thread_count; i++) {
		tickle();
	}

	if(m_root_fiber) {
		if(!stopping()) {
			m_root_fiber->call();
		}
	}

	std::vector<Thread::ptr> thrs;
	{
		MutexType::Lock lock(m_mutex);
		thrs.swap(m_thread_pool);
	}
	for(auto& i : thrs) {
		i->join();
	}
}

// 协调线程和协程之间的关系, 无任务时线程做什么事情
void scheduler::run() {
	// 取出需要执行的协程
	TT_DEBUG << "scheduler: " << m_name << "->run: ";
	set_this();
		// 
	if(GetThreadId() != m_root_thread) {
		t_scheduler_fiber = Fiber::GetThis();
	}


		// 如果没有需要执行的任务则执行 idle
	Fiber::ptr idle_fiber(new Fiber(std::bind(&scheduler::run, this)));
	Fiber::ptr cb_fiber;
	// ft.fiber
	FiberAndThread ft;
	while(1) {  // 不断执行m_fibers
		ft.reset();
		bool tickle_me = false;
		bool is_active = false;
		{
			MutexType::Lock lock(m_mutex);
			auto it = m_fibers.begin();
			while(it != m_fibers.end()) {
				if(it->thread != -1 && it->thread != GetThreadId()) {  // 并非此线程需要执行的task
					++it;
					tickle_me = true;
					continue;
				}

				// 找到了一个需要在本线程执行的 Fiber || func
				TT_ASSERT(it->fiber || it->cb);
				if(it->fiber && it->fiber->get_state() == Fiber::EXEC) {  // 协程正在执行
					++it;
					continue;
				}

				ft = *it;
				m_fibers.erase(it++);
				++m_active_thread_count;
				is_active = true;
				break;
			}
			tickle_me |= it != m_fibers.end();
		}

		if(tickle_me) {
			tickle();
		}

		if(ft.fiber && (ft.fiber->get_state() != Fiber::TERM
					      && ft.fiber->get_state() != Fiber::EXCEPT)) {
			ft.fiber->swapin();
			--m_active_thread_count;

			if(ft.fiber->get_state() == Fiber::READY) {
				schedule(ft.fiber);
			} else if(ft.fiber->get_state() != Fiber::TERM
					   && ft.fiber->get_state() != Fiber::EXCEPT) {
				ft.fiber->m_state = Fiber::HOLD;
			}
			ft.reset();
		} else if(ft.cb) { // cb
			if(cb_fiber) {
				cb_fiber->reset(ft.cb);
			} else {
				cb_fiber.reset(new Fiber(ft.cb));
			}
			ft.reset();
			cb_fiber->swapin();
			--m_active_thread_count;
			// 协程执行完毕状态为 TERM || EXCEPT
			if(cb_fiber->get_state() == Fiber::EXCEPT
					|| cb_fiber->get_state() == Fiber::TERM) {
				cb_fiber->reset(nullptr);
			} else {
				cb_fiber->m_state = Fiber::HOLD;
				cb_fiber.reset();
			}
		} else { // idle
			if(is_active) {
				--m_active_thread_count;
				continue;
			}
			if(idle_fiber->get_state() == Fiber::TERM) {
				TT_DEBUG << "idle fiber term";
				break;
			}
			++m_idle_thread_count;
			idle_fiber->swapin();
			--m_idle_thread_count;
			if(idle_fiber->get_state() != Fiber::TERM
					&& idle_fiber->get_state() != Fiber::EXCEPT) {
				idle_fiber->m_state = Fiber::HOLD;
			}
		}
	}
	//
}   // 协程调度

void scheduler::tickle() {
	TT_DEBUG << "scheduler: "<<  m_name << "tickle!";
}  // notice task


bool scheduler::stopping() {
	MutexType::Lock lock(m_mutex);
	return m_auto_stop && m_stopping
		&& m_fibers.empty() && m_active_thread_count == 0;
}  // 是否可以停止

// 虽然没有任务做, 但是占用CPU
void scheduler::idle() {
	TT_DEBUG << "idle";
	while(stopping()) {
		Fiber::YeldToHold();
	}
}	  // 无task时idle
void scheduler::set_this() {
	t_scheduler = this;

}		  // 设置当前协程调度器



} // namespace system
} // namespace system


