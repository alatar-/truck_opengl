#ifndef _INCLUDE_MODEL_H
#define _INCLUDE_MODEL_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include "shared.h"

#include "mesh.h"
#include "material.h"
#include "vertex.h"

using namespace std;

class model_t {
protected:
	vector <mesh_t*> meshes;
	vector <material_t*> materials;
	
	virtual bool parse_scene (const aiScene *pScene, string &model_file);
public:
	float width;
	float length;
	vertex_2d overall_min_point, overall_max_point;
	model_t();
	virtual ~model_t();
	bool load_materials (const aiScene* pScene, const string& model_file);
	virtual void clear();
	bool load (string model_file);
	
	vector <material_t*> &get_materials() {
		return materials;
	}
	
	void set_mv_matrix (glm::mat4 MV);
	
	void draw (bool use_mv = false, bool apply_material = true);
	
	bool test_intersection (vertex_3d pos0, vertex_3d pos1);
	float get_width();
	float get_length();
	vertex_2d get_min_point();
	vertex_2d get_max_point();
};

#endif
