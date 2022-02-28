#include "timer.h"
#include <inttypes.h>
#include <stdio.h>


timer* create_timer(uint64_t time, void (*cb)()) {
	timer* ans = (timer*)malloc(sizeof(timer));
	
	ngx_rbtree_node_t* node = (ngx_rbtree_node_t*)

	node->key = time + cur_time_ms();
	node->left = node->right = node->parent = NULL;

	ans->cb = cb;
	return ans;
}

void add_timer(timer_manager* tm, uint64_t time_ms, void (*cb)()) {
	ngx_rbtree_node_t* rn = (ngx_rbtree_node_t*)create_timer(time_ms, cb);
	ngx_rbtree_insert(&tm->tree, rn);
}

void del_timer(timer_manager* tm, timer* t) {
	if(&t->rbtree == tm->tree.sentinel) {
		printf("can^t del sentinel node\n");
		return ;
	}
	ngx_rbtree_node_t* rn = &t->rbtree;
	ngx_rbtree_delete(&tm->tree, rn);
	free(t);
}

uint64_t timer_time(timer* t) {
	return t->rbtree.key;
}

void init_timer_manager(timer_manager* tm) {
	timer* sentinel = create_timer(0, NULL);
	sentinel->rbtree.left = sentinel->rbtree.right = &sentinel->rbtree;

	ngx_rbtree_init((rbtree_t*)tm, (rbtree_node_t*)sentinel, ngx_rbtree_insert_timer_value);
	

	tm->cur_time = &cur_time_ms;
	tm->create_timer = &create_timer;

	tm->add = &add_timer;
	tm->del = &del_timer;
	tm->min = &get_min;
	tm->time = &timer_time;
	tm->handle_timer = &handle_timer;
}

timer* get_min(timer_manager* tm) {
	timer *ans = NULL;
	rbtree_t* tree = (rbtree_t*)tm;
	ans = (timer*)ngx_rbtree_min(tree->root, tree->sentinel);
	return ans;
}

void handle_timer(timer* t) {
	while(cur_time_ms() < t->rbtree.key) {
	}
	t->cb();
}

