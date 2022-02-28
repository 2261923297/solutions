#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <functional>
#include <ucontext.h>

namespace tt { 

class MallocStackAllocator {
public:
	static void* Alloc(size_t size) { return malloc(size); }
	static void  Dealloc(void* stack, size_t size = 0) {
		free(stack);
	}
private:
	
}; // class MallocStackAllocator	
/**
 *		每个协程都有一个主协程, 负责调度子协程, 
 *		子协程只能执行完毕回到主协程
 *
 * */
/**
 *		持久性的学习
 *		封装学了没有地方用到
 *		封装一个log 用于调试， 知识的连贯性，可用性， 可持续性 
 * */
class Fiber : public std::enable_shared_from_this<Fiber> { 
public: 
	using ptr = std::shared_ptr<Fiber>;
	Fiber(std::function<void()> cb, size_t stack_size = 0);
	~Fiber();

	enum State { INIT, HOLD, EXEC, TERM, READY, EXCEPT };	

private:
	Fiber();

public:
	uint64_t get_id() const { return m_id; }
	void swapin();

	void swapout();

	void reset(std::function<void()> cb);
public:
	static void SetThis(Fiber* fb);
	static Fiber::ptr GetThis();

	static void YeldToHold();

	static void YeldToReady();

	static uint64_t TotalFibers();

	static uint64_t GetFiberId();

	static void MainFunc();
private:
	uint64_t m_id = 0;
	State    m_state = INIT;

	ucontext_t m_ctx;
	std::function<void()> m_cb;

	uint32_t m_stack_size = 0;
	void*    m_stack = nullptr;

}; // Fiber

} // namespcae tt 


