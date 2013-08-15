#ifndef MESH_H
#define MESH_H

#include "shared.h"

class Material;
#include "material.h"
#include "extra_vertex.h"
#include <vector>

using namespace std;

class Mesh {
private:
	float *pos_data, *norm_data, *tex_coords;
	vector <unsigned> indices;
	vector <ExtraVertex> vertices;
	Material *material;
	
	Vertex2D<float> cords_min, cords_max;
	
	glm::mat4 MV;
public:
	Mesh (const aiMesh *paiMesh, Material *in_material);
	~Mesh();

	void set_vertices_data();
	void draw (bool use_mv, bool apply_material, glm::mat4 V = glm::mat4(1.0f));
	
	ExtraVertex& operator[] (unsigned i);
	unsigned vertices_size();
	
	void set_mv_matrix (glm::mat4 in_MV);
	void set_material (Material *in_material);
	
	void print();
	
	Vertex2D<float> get_cords_min();
	Vertex2D<float> get_cords_max();
};

#endif
