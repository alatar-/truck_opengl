#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "vertex.h"
#include <vector>

using namespace std;

class Rectangle {
private:
	vertex_2d UL, UR, LL, LR;
	vertex_2d projection[4];
	vertex_2d Axis[2];
	float points[4];
public:
	Rectangle();
	Rectangle(vertex_2d in_ul, vertex_2d in_ur, vertex_2d in_ll, vertex_2d in_lr);
	~Rectangle();
	bool intersection(Rectangle &rect);
	bool full_inclusion(Rectangle &rect);
	bool has_point_inside(vertex_2d &in_vertex);
	void set_vertices(vector <vertex_2d> in);
	void print();
};

#endif
