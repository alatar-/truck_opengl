#ifndef EXTRA_VERTEX_H
#define EXTRA_VERTEX_H

#include "vertex.h"
#include "shared.h"
#include <vector>
#include <utility>

using namespace std;

class ExtraVertex {
private:
	Vertex3D<float> pos
		,	norm;
public:
	ExtraVertex(Vertex3D<float> in_pos, Vertex3D<float> in_norm);
	Vertex3D<float> get_pos();
	Vertex3D<float> get_norm();
	
	void set_pos (Vertex3D<float> in_pos);
	void set_norm (Vertex3D<float> in_norm);
};

#endif
