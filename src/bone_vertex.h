#ifndef _INCLUDE_BONE_VERTEX_H
#define _INCLUDE_BONE_VERTEX_H

#include "vertex.h"
#include "shared.h"
#include <vector>
#include <utility>

using namespace std;

class bone_vertex_t {
private:
	Vertex3D<float> pos
		,	norm;
public:
	bone_vertex_t(Vertex3D<float> in_pos, Vertex3D<float> in_norm);
	~bone_vertex_t();
	Vertex3D<float> get_pos();
	Vertex3D<float> get_norm();
	
	void set_pos (Vertex3D<float> in_pos);
	void set_norm (Vertex3D<float> in_norm);
};

#endif
