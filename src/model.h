#ifndef _INCLUDE_MODEL_H
#define _INCLUDE_MODEL_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include "shared.h"

#include "mesh.h"
#include "material.h"

using namespace std;

class model_t {
protected:
	vector <mesh_t*> meshes;
	vector <material_t*> materials;
	
	virtual bool parse_scene (const aiScene *pScene, string &model_file);
public:
	model_t();
	virtual ~model_t();
	bool load_materials (const aiScene* pScene, const string& model_file);
	virtual void clear();
	bool load (string model_file);
	
	vector <material_t*> &get_materials() {
		return materials;
	}
	
	void set_mv_matrix (glm::mat4 MV);
	
	void draw (bool use_mv = false);
	
	bool test_intersection (vertex_3d pos0, vertex_3d pos1);
};

#endif
