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
	vector <Mesh*> meshes;
	vector <Material*> materials;
	
	virtual bool parse_scene (const aiScene *pScene, string &model_file);
public:
	float width;
	float length;
	Vertex2D<float>overall_min_point, overall_max_point;
	model_t();
	virtual ~model_t();
	bool load_materials (const aiScene* pScene, const string& model_file);
	virtual void clear();
	bool load (string model_file);
	
	vector <Material*> &get_materials() {
		return materials;
	}
	
	void set_mv_matrix (glm::mat4 MV);
	
	void draw (bool use_mv = false, bool apply_material = true);
	
	float get_width();
	float get_length();
	Vertex2D<float>get_min_point();
	Vertex2D<float>get_max_point();
};

#endif
