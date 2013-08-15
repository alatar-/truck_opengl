#ifndef _INCLUDE_PATH_H
#define _INCLUDE_PATH_H

#include <vector>
#include <string>
#include "vertex.h"

#define STOP_AND_STARE true
#define STILL_MOVING false

using namespace std;

class path_t {
private:
	vector <vector <vertex_2d<float> > points;
	vertex_2d<float>pos;
	int next;
	int array;
	float ang_h;
	bool loop
		,	_step;
	
	void inc_by_step (int &number);
	void inc_next();
	void inc_array();
	void set_first();
	bool next_out_of_bounds();
	bool extremal();
public:
	path_t (string in_arr, bool looped);
	~path_t();
	bool move (float step);
	vertex_2d<float>position();
	float rotation();
	
	void print();
};

#endif
