#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include "shared.h"

#include "mesh.h"
#include "material.h"
#include "vertex.h"

using namespace std;

class Model {
protected:
	vector <Mesh*> meshes;
	vector <Material*> materials;
	
	virtual bool parse_scene (const aiScene *pScene, string &model_file);
public:
	float width;
	float length;
	Vertex2D<float>overall_min_point, overall_max_point;
	Model();
	virtual ~Model();
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
