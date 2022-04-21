#pragma once
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "ngx_rbtree.h"
#include "util.h"


typedef struct ngx_rbtree_s rbtree_t;
typedef struct ngx_rbtree_node_s rbtree_node_t;
	

typedef struct timer {
	ngx_rbtree_node_t rbtree;
	void (*cb)();

} timer;

typedef struct timer_manager {	
	rbtree_t tree;

	uint64_t (*cur_time)();
	timer* (*create_timer)(uint64_t time, void (*cb)());

	void (*add)(struct timer_manager* tm, uint64_t time, void (*cb)());
	void (*del)(struct timer_manager* tm, timer* t) ;

	timer* (*min)(struct timer_manager* tm);
	void (*handle_timer)(timer* t);
	uint64_t (*time)(timer* t);
} timer_manager;

timer* create_timer(uint64_t time, void (*cb)());

void add_timer(timer_manager* tm, uint64_t time, void (*cb)());
void del_timer(timer_manager* tm, timer* t);
timer* get_min(timer_manager* tm); 
uint64_t timer_time(timer* t);

void init_timer_manager(timer_manager* tm); 
void handle_timer(timer* t);

