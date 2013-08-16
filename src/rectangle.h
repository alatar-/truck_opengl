#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "vertex.h"
#include <vector>

using namespace std;

class Rectangle {
private:
	Vertex2D <float> UL, UR, LL, LR;
public:
    Rectangle(){};
	Rectangle(Vertex2D <float> in_ul, Vertex2D <float> in_ur, Vertex2D <float> in_ll, Vertex2D <float> in_lr);
	bool intersection(Rectangle &rect, Vertex2D <float> &out_axis);
	bool full_inclusion(Rectangle &rect);
	bool has_point_inside(Vertex2D <float> &in_vertex);
	void set_vertices(vector <Vertex2D <float> > in);
	vector <Vertex2D<float> > get_vertices();
	void print();
};

#endif
