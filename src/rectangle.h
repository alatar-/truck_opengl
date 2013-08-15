#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "vertex.h"
#include <vector>

using namespace std;

class Rectangle {
private:
	vertex_2d<float>UL, UR, LL, LR;
public:
	Rectangle();
	Rectangle(vertex_2d<float>in_ul, vertex_2d<float>in_ur, vertex_2d<float>in_ll, vertex_2d<float>in_lr);
	~Rectangle();
	bool intersection(Rectangle &rect);
	bool full_inclusion(Rectangle &rect);
	bool has_point_inside(vertex_2d<float>&in_vertex);
	void set_vertices(vector <vertex_2d<float> > in);
	vector <vertex_2d<float> > get_vertices();
	void print();
};

#endif
