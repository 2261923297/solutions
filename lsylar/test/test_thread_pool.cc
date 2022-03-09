#include "thread_pool.h"
#include <unistd.h>

void tp_func() {
	printf(__FUNCTION__ " begin\n");
	sleep(1000);
	printf(__FUNCTION__ " end\n");
}

int main() {

	int thread_num = 10;
	thread_pool tp;
	for(int i = 0; i < thread_num ; i++) { 
		tp.add_task(tp_func);
	}
	

	return 0;
}
