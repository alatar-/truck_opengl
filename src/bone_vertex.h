#ifndef _INCLUDE_BONE_VERTEX_H
#define _INCLUDE_BONE_VERTEX_H

#include "vertex.h"
#include "shared.h"
#include <vector>
#include <utility>

using namespace std;

class bone_vertex_t {
private:
	vertex_3d	pos
		,	norm;
public:
	bone_vertex_t(vertex_3d in_pos, vertex_3d in_norm);
	~bone_vertex_t();
	vertex_3d get_pos();
	vertex_3d get_norm();
	
	void set_pos (vertex_3d in_pos);
	void set_norm (vertex_3d in_norm);
};

#endif
