#include "thread_pool.h"

void* thrd_func(void* arg) {
	sem_t* sem = (sem_t*)arg;
	sleep(1);
	printf("thread end");
	sem_post(sem);
	
}


int main() {
	sem_t sem ;
	sem_init(&sem, 0, 10);

	thread_pool tp;
	tp.add_func(thrd_func, args);

	return 0;
}
