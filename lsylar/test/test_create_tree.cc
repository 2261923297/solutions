#include "create_tree.h"
#include "util.h"

#include <iostream>
#include <map>
#include <vector>
#include <list>

#define side	std::map<int, int>
#define point	std::pair<int, side*>

#define Grap	std::vector<point*> 

void add_side(point* p, int point_number, int weight) {
	p->second->insert(std::pair<int, int>(point_number, weight));

}

void add_point(Grap& g, point* p) {
	g.push_back(p);
}

void print_grap(Grap& g) {
	for(auto pt : g) {
		printf("point: %d, <side, weight>: ", pt->first);
		for(auto pr : *pt->second) {
			printf("<%d, %d>, ", pr.first, pr.second);
		}
		printf("\n");
	}
}

point* init_p0() {
	point* po = new point({0, new side});
	add_side(po, 2, 26);
	add_side(po, 4, 38);
	add_side(po, 6, 58);
	add_side(po, 7, 34);
	return po;
}
point* init_p1() {
	point* p1 = new point({1, new side});
	add_side(p1, 2, 36);
	add_side(p1, 3, 29);
	add_side(p1, 5, 32);
	add_side(p1, 7, 19);
	return p1;
}

point* init_p2() {
	point* p2 = new point({2, new side});
	add_side(p2, 0, 26);
	add_side(p2, 1, 36);
	add_side(p2, 3, 29);
	add_side(p2, 6, 40);
	add_side(p2, 7, 34);
	return p2;
}

point* init_p3() {
	point* p3 = new point({3, new side});
	add_side(p3, 1, 29);
	add_side(p3, 2, 17);
	add_side(p3, 6, 52);
	return p3;
}

point* init_p4() {
	point* p4 = new point({4, new side});
	add_side(p4, 0, 38);
	add_side(p4, 5, 35);
	add_side(p4, 6, 29);
	add_side(p4, 7, 37);
	return p4;
}

point* init_p5() {
	point* p5 = new point({5, new side});
	add_side(p5, 1, 32);
	add_side(p5, 4, 35);
	add_side(p5, 7, 37);
	return p5;
}

point* init_p6() {
	point* p6 = new point({6, new side});
	add_side(p6, 0, 58);
	add_side(p6, 2, 40);
	add_side(p6, 3, 52);
	add_side(p6, 4, 29);
	return p6;
}

point* init_p7() {
	point* p7 = new point({7, new side});
	add_side(p7, 0, 16);
	add_side(p7, 1, 19);
	add_side(p7, 2, 34);
	add_side(p7, 4, 37);
	add_side(p7, 5, 26);
	return p7;
}
void init_grap(Grap& g) {
	add_point(g, init_p0());
	add_point(g, init_p1());
	add_point(g, init_p2());
	add_point(g, init_p3());
	add_point(g, init_p4());
	add_point(g, init_p5());
	add_point(g, init_p6());
	add_point(g, init_p7());
}

#define side_in_tree_t std::vector<std::tuple<int, int, int>>
int total_print = 0;
int full_print(side_in_tree_t& vec) {
	if(vec.size() == 7) {
		for(auto it : vec) {
			// side: weight
			printf("%d-%d: %2d   ", 
				std::get<0>(it), std::get<1>(it), std::get<2>(it));

		}
		printf("\n");

		total_print++;

	}
	return 0;
}

int have_side(point* p, char hp[8]) {
	for(auto it : *p->second) {
		if(hp[it.first] == 0)
			return it.first;
	}
	return -1;
}

int no_this_point(side_in_tree_t& vec, int point_num) {
	for(auto it : vec) {
		if(std::get<1>(it) == point_num || std::get<0>(it) == point_num)
			return 0;
	}
	return 1;
}

#define branch_t	 std::vector<std::pair<int, int>>
#define branch_set_t std::vector<branch_t*>

void print_vec(side_in_tree_t& vec) {
	for(auto it : vec)	{
		// side: weight
		printf("%d-%d: %2d   ", 
			std::get<0>(it), std::get<1>(it), std::get<2>(it));
	}
	printf("\n");
}
int get_point(side_in_tree_t*);
void pp(Grap& g, side_in_tree_t& vec, char hp[8], point* p) {
//	print_vec(vec);
	int end = have_side(p, hp);
	if(-1 == end) { 
		full_print(vec);
	} else {
		int n_add = 0;
		for(auto sd : *p->second) {
			if(no_this_point(vec, sd.first)) {
				hp[sd.first] = 1;
				vec.push_back(std::make_tuple(p->first, sd.first, sd.second));  // 修改集合
				n_add++;
			}
		}
		while(n_add--) {
			int sz = vec.size() - 1;
			point* ep = NULL;
			while(sz >= 0 && 
				p == (ep = g[std::get<0>(vec[sz])])) {
				ep = g[std::get<1>(vec[sz])];
				pp(g, vec, hp, ep);
				sz--;
			}
			hp[std::get<1>(vec.back())] = 0;
			vec.pop_back();

		}


	}
}


void print_tree(Grap& g) {
	char have_point[8] = { 0 };

	// typle<side, side, weight>
	side_in_tree_t nums;
	/*
	nums.push_back(std::make_tuple(5, 1, 32));
	nums.push_back(std::make_tuple(5, 4, 35));
	nums.push_back(std::make_tuple(5, 7, 37));

	have_point[5] = 1;
	have_point[1] = have_point[4] = have_point[7] = 1;
	*/
	pp(g, nums, have_point, g[5]);
	print_val(total_print, d);
}
int main() {
	Grap g;
	init_grap(g);
	print_grap(g);
	print_tree(g);

	return 0;
}
