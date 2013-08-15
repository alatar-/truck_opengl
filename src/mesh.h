#ifndef _INCLUDE_MESH_H
#define _INCLUDE_MESH_H

#include "shared.h"

class material_t;
#include "material.h"
#include "extra_vertex.h"
#include <vector>

using namespace std;

class mesh_t {
private:
	float *pos_data, *norm_data, *tex_coords;
	vector <unsigned> indices;
	vector <ExtraVertex> vertices;
	material_t *material;
	
	Vertex2D<float> cords_min, cords_max;
	
	glm::mat4 MV;
public:
	mesh_t (const aiMesh *paiMesh, material_t *in_material);
	~mesh_t();

	void set_vertices_data();
	void draw (bool use_mv, bool apply_material, glm::mat4 V = glm::mat4(1.0f));
	
	ExtraVertex& operator[] (unsigned i);
	unsigned vertices_size();
	
	void set_mv_matrix (glm::mat4 in_MV);
	void set_material (material_t *in_material);
	
	void print();
	
	Vertex2D<float> get_cords_min();
	Vertex2D<float> get_cords_max();
};

#endif
