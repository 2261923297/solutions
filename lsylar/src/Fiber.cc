#include "Fiber.h" 
#include <errno.h>
#include <atomic>
#include "macro.h"

namespace tt {  
namespace system {

static std::atomic<uint64_t> s_fiber_id { 0 };
static std::atomic<uint64_t> s_fiber_count { 0 };

static thread_local Fiber* t_fiber = nullptr;

static uint32_t g_fiber_stack_size = 1024 * 1024;

using StackAllocator = MallocStackAllocator;

// main fiber
static thread_local Fiber* t_thread_fiber = nullptr;

void main_fiber_cb() {
	TT_DEBUG << "main_fiber_cb" << std::endl;
}

Fiber::Fiber() {  // 主协程
	m_state = EXEC;
	SetThis(this);

	if(getcontext(&m_ctx)) {
		TT_DEBUG << "getcontext error";
		TT_ASSERT(false);
	}
	++s_fiber_count;

} 
Fiber::Fiber(std::function<void()> cb, size_t stack_size) 
	: m_id(++s_fiber_id)
	, m_cb(cb) {
	++s_fiber_count;

	m_stack_size = stack_size ? stack_size : g_fiber_stack_size;
	
	m_stack = StackAllocator::Alloc(m_stack_size);
	if(getcontext(&m_ctx)) {
		TT_DEBUG << "getcontext error";
		TT_ASSERT(false);
	}
	m_ctx.uc_link = nullptr;
	m_ctx.uc_stack.ss_sp = m_stack;
	m_ctx.uc_stack.ss_size = m_stack_size;

	makecontext(&m_ctx, &Fiber::MainFunc, 0);

}

Fiber::~Fiber() {
	--s_fiber_count;
	std::cout << "m_ctx = " <<&m_ctx 
		<< ", fiber count = " << s_fiber_count
		<< std::endl;
	if(m_stack) {
		TT_DEBUG << "state: " << m_state;
		StackAllocator::Dealloc(m_stack, m_stack_size);
	} else {
		TT_DEBUG << "MainFiber is end, state = " << m_state;
		if(t_fiber == this) {
			SetThis(nullptr);
		}
	}
}

std::string pt_data() {
	std::stringstream ss;
	ss << "t_thread_fiber = " << t_thread_fiber->get_ctx()
		<< ", t_fiber = " << t_fiber->get_ctx() ;

	return ss.str();
}
// 唤醒执行 cb
void Fiber::swapin() {
//	TT_DEBUG << "swapin: " <<  pt_data();
	SetThis(this);
	if(m_state == EXEC) {
		TT_DEBUG << "can^t swapin error state";
		return;
	}
	m_state = EXEC;

//	TT_DEBUG << "swapin: " << pt_data();

	if(swapcontext((t_thread_fiber->get_ctx()), &m_ctx)) {
//		TT_DEBUG << "can^t swapcontext errno = " << errno;
	} 

}

// 唤醒main fiber
void Fiber::swapout() {
	SetThis(t_thread_fiber);

	m_state = TERM;
	if(swapcontext(&m_ctx, &t_thread_fiber->m_ctx)) {
		TT_DEBUG << "can^t swapout errno = " << errno;
	}
}

void Fiber::call() {
	SetThis(this);
	m_state = EXEC;
	if(swapcontext(t_thread_fiber->get_ctx(), &m_ctx)) {
		TT_ASSERT(false);
	}
		
}
void Fiber::back() {
	SetThis(t_thread_fiber);
	if(swapcontext(&m_ctx, t_thread_fiber->get_ctx())) {
		TT_ASSERT(false);
	}
}

void Fiber::reset(std::function<void()> cb) {
	if(m_state == TERM || m_state == INIT || m_state == EXCEPT) {
		TT_DEBUG << "m_state error ";
	} 
	m_cb = cb;
	if(getcontext(&m_ctx)) {
		TT_DEBUG << "getcontext error";
		TT_ASSERT(false);
	}
	m_ctx.uc_link = nullptr;
	m_ctx.uc_stack.ss_sp = m_stack;
	m_ctx.uc_stack.ss_size = m_stack_size;

	makecontext(&m_ctx, Fiber::MainFunc, 0);
}

void Fiber::SetThis(Fiber* fb) {
	t_fiber = fb; 
}

Fiber* Fiber::GetThis() {
	if(t_fiber != nullptr) {
		return t_fiber;
	}
	// 创建主协程
	Fiber* main_fiber = new Fiber;
	if(t_fiber != main_fiber) {
		TT_DEBUG << "t_fiber != main_fiber.get()";
	}
	t_thread_fiber = main_fiber; 
	return t_fiber;
}

void Fiber::YeldToHold() {
	Fiber* cur = GetThis();
	cur->m_state = HOLD;
	cur->swapout();
}

void Fiber::YeldToReady() {
	Fiber* cur = GetThis();
	cur->m_state = READY;
	cur->swapout();

}

uint64_t Fiber::TotalFibers() {
	return s_fiber_count;
}

void Fiber::MainFunc() {
	Fiber* cur = GetThis();

	if(nullptr == cur) {
		TT_DEBUG << "error cur";
	}
	try {
		cur->m_cb();
		cur->m_cb = nullptr;
		cur->m_state = TERM;
	} catch(std::exception& ex) {
		cur->m_state = EXCEPT;
		TT_DEBUG << "MainFunc occur except: " << ex.what();
	} catch(...) {
		TT_DEBUG << "MainFunc occur except: ";
	}
}
uint64_t Fiber::GetFiberId() {
	if(t_fiber) {
		return t_fiber->get_id();
	}
	return 0;
}

} // namespace system

} // namespace tt
