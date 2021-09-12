#include "Thread.h"
#include <errno.h>

#include <unistd.h> 
struct st_add {
	int a;
	int b;
	int sum;
	tt::Semaphore *sem;
};

void
thread_func()
{
	std::cout << "thread_func" << std::endl;

	tt::Thread* thread = tt::Thread::GetThis();
	
	std::cout << "thread_id:" << thread->getId()
			<< "\nsyscall_id: " << syscall(SYS_gettid)
			<< "\nthread_name: " << thread->getName()
			<< "\nthread_pthread: " << thread->getPthread()
			<< "\npthread_self:   " << pthread_self()
			<< std::endl;
}

void 
test_pthread() {
	tt::Thread t(thread_func, "test_thrd");

}
    
void 
test_shared_ptr() {
    
}

int main() {
	test_pthread();

	tt::Thread::SetName("main");

	
	std::cout << "\n"
			<< "\nsyscall_id: " << syscall(SYS_gettid)
			<< "\nthread_name: " << tt::Thread::GetName()
			<< "\npthread_self:   " << pthread_self()
			<< std::endl;

    std::cout << "Hello, test Thread base!" << std::endl;

    return 0;
}
    
