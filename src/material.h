#ifndef _INCLUDE_MATERIAL_H
#define _INCLUDE_MATERIAL_H

#include <string>
#include <cstring>
#include <vector>
#include "shared.h"
#include "texture.h"

class Mesh;
#include "mesh.h"

using namespace std;

class Material {
private:
	Texture *texture;
	float ambient[4]
		,	emission[4]
		,	diffuse[4]
		,	specular[4]
		,	shininess;
	
	double texture_size;
public:
	Material();
	~Material();
	
	vector <Mesh*> meshes;
	void clear();
	bool load (aiMaterial *material, string &dir);
	void apply();
	
	void draw_associated_meshes(glm::mat4 V = glm::mat4(1.0f));
	
	double get_texture_size() {
		return texture_size;
	}
	
	string get_texture_file_name();
	int compare_textures(const Material &o) const;
	bool operator<(const Material &o) const;
	bool operator==(const Material &o) const;

	void substitute(Material *o);
};

#endif
