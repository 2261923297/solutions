#include  "timer.h"


void timer_cb() {
	print_llu(cur_time_ms());
}

void sentinel_cb() {
	printf("sentinel_cb\n");
}

void test_timer() {	
	timer_manager* tm = (timer_manager*)malloc(sizeof(timer_manager));

	init_timer_manager(tm);

	tm->add(tm, 1000, timer_cb);
	timer *min = tm->min(tm);

	handle_timer(min);
	tm->del(tm, min);

	min = tm->min(tm);
	tm->del(tm, min);
	
}
void test() {
	ngx_rbtree_node_t sentinel;

	rbtree_t tr;
	rbtree_node_t rn;
	rn.key = 1;
	ngx_rbtree_init(&tr, &sentinel, ngx_rbtree_insert_timer_value);
	
	ngx_rbtree_insert(&tr, &rn);
	ngx_rbtree_delete(&tr, &rn);
	
}

int main() {
	printf("hello, timer\n");
	test_func(test);

	test_func(test_timer);
	return 0;
}
