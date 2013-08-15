#include "extra_vertex.h"

ExtraVertex::ExtraVertex(Vertex3D<float> in_pos, Vertex3D<float> in_norm) {
	pos = in_pos;
	norm = in_norm;
}

ExtraVertex::~ExtraVertex() {

}


aiMatrix4x4 ai_M_mul(const aiMatrix4x4 &M, float a) {
	aiMatrix4x4 I;
	for (unsigned i = 0; i < 4; ++i) {
		for (unsigned j = 0; j < 4; ++j) {
			I[i][j] = M[i][j] * a;
		}
	}
	return I;
}

void ai_M_add(aiMatrix4x4 &M, const aiMatrix4x4 &A) {
	for (unsigned i = 0; i < 4; ++i) {
		for (unsigned j = 0; j < 4; ++j) {
			M[i][j] += A[i][j];
		}
	}
}

Vertex3D<float> ExtraVertex::get_pos() {
	return pos;
}

Vertex3D<float> ExtraVertex::get_norm() {
	return norm;
}

void ExtraVertex::set_pos (Vertex3D<float> in_pos) {
	pos = in_pos;
}

void ExtraVertex::set_norm (Vertex3D<float> in_norm) {
	norm = in_norm;
}
