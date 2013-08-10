#include "path.h"
#include "shared.h"
#include <cstdio>

path_t::path_t (string in_arr, bool looped) {
	in_arr = get_file(in_arr.c_str());
// 	printf("0 lev> %s\n", in_arr.c_str());
	size_t idx = in_arr.find("[", 0, 1);
	if (idx != string::npos) {
// 		printf("1 lev\n");
		for (idx = in_arr.find("[", idx + 1, 1)
			;	idx != string::npos
			;	idx = in_arr.find("[", idx + 1, 1)) {
			
// 			printf("2 lev> idx = %u\n", idx);
			idx += 1;
			size_t idx2 = in_arr.find("]", idx, 1);
			if (idx2 == string::npos) {
				idx2 = in_arr.find("[", idx, 1) - 1;
			}
			
// 			printf("3 lev: idx2 = %u\n", idx2);
			if (idx2 != string::npos) {
				string pts = in_arr.substr(idx, idx2 - idx);
				points.push_back(vector <vertex_2d>());
				vector <vertex_2d> &arr = points.back();
				
// 				printf("4 lev\n");
				for (size_t jdx = pts.find("(", 0, 1)
					;	jdx != string::npos
					;	jdx = pts.find("(", jdx + 1, 1)
				) {
// 					printf("5 lev\n");
					float x = 0
						,	y = 0;
					jdx += 1;
					size_t jdx2 = pts.find(")", jdx, 1);
					
					if (jdx2 != string::npos) {
						sscanf(pts.substr(jdx, jdx2 - jdx).c_str(), " %f, %f", &x, &y);
						arr.push_back(vertex_2d(x, y));
					}
					jdx = jdx2;
				}
			} else {
				break;
			}
			idx = idx2;
		}
	}
	
	if (points.empty()) {
		points.push_back(vector <vertex_2d>());
	}
	if (points[0].empty()) {
		points[0].push_back(vertex_2d(0.f, 0.f));
	}
	
	array = 0;
	next = 0;
	loop = looped;
	_step = true;
	ang_h = 0;
	pos = points[0][0];
	
// 	printf("constructed, pos=(%f, %f)\n", pos.x, pos.y);
}

path_t::~path_t() {
}

void path_t::inc_by_step (int &number) {
	if (_step) {
		number += 1;
	} else {
		number -= 1;
	}
}

void path_t::inc_next() {
	inc_by_step(next);
}

void path_t::inc_array() {
	inc_by_step(array);
	
	if (array >= (int)points.size()) {
		if (loop) {
			array = 0;
		} else {
			_step = false;
			array = points.size() - 1;
		}
	} else if (array < 0) {
		array = 0;
		_step = true;
	}
	
	set_first();
}

void path_t::set_first() {
	if (_step) {
		next = 0;
	} else {
		next = (int)points[array].size() - 1;
	}
}

bool path_t::next_out_of_bounds() {
	return next < 0
		|| next >= (int)points[array].size();
}

bool path_t::extremal() {
	return next == 0
		|| next == (int)points[array].size() - 1;
}


bool path_t::move (float step) {
// 	printf("path_t::move(%f), next=%d, array=%d\n", step, next, array);
	if (step <= 0) {
		return STILL_MOVING;
	}
	
	if (next_out_of_bounds()) {
// 		printf("path_t::move> next out of bounds\n");
		inc_array();
		return STOP_AND_STARE;
	}
	
	vector <vertex_2d> &p = points[array];
	vertex_2d pnext = p[next];
	if (extremal() && pos == pnext) {
// 		printf("path_t::move> extremal\n");
		inc_next();
		return STOP_AND_STARE;
	}
	
	float dx = pnext.x - pos.x
		,	dy = pnext.y - pos.y
		,	dist = module(dx, dy);
	ang_h = atan2(dx, dy);
	
// 	printf("path_t::move> pnext=(%f, %f)\n", pnext.x, pnext.y);
	if (dist > step) {
		float scale = step / dist;
		pos.x += dx * scale;
		pos.y += dy * scale;
// 		printf("path_t::move> pos=(%f, %f)\n", pos.x, pos.y);
	} else {
		pos = pnext;
		inc_next();
// 		printf("path_t::move> inc, next=%d\n", next);
		return move(step - dist);
	}
	
// 	printf("path_t::move> returning false\n");
	return STILL_MOVING;
}

vertex_2d path_t::position() {
	return pos;
}

float path_t::rotation() {
	return ang_h * 180 / PI;
}

void path_t::print() {
	printf("[\n");
	for (unsigned i = 0, ilen = points.size(); i < ilen; ++i) {
		printf("[");
		for (unsigned j = 0, jlen = points[i].size(); j < jlen; ++j) {
			printf(" (%.2f, %.2f)", points[i][j].x, points[i][j].y);
		}
		printf("]\n");
	}
	printf("]\n");
}
