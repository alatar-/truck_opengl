#include "material.h"

void load_color (aiMaterial *material, const char *pKey, unsigned type, unsigned idx, float *store, float dflt) {
	aiColor3D color (0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	if ( color.IsBlack() ) {
		store[0] = dflt;
		store[1] = dflt;
		store[2] = dflt;
	} else {
		store[0] = color.r;
		store[1] = color.g;
		store[2] = color.b;
	}
}

material_t::material_t() {
	texture = NULL;
	ambient[3] = 1;
	emission[3] = 1;
	diffuse[3] = 1;
	specular[3] = 1;
	shininess = 0;
}

bool material_t::load (aiMaterial *material, string &dir) {
	bool res = true;
	
	clear();
	
	load_color(material, AI_MATKEY_COLOR_AMBIENT, ambient, 0.5);
	load_color(material, AI_MATKEY_COLOR_EMISSIVE, emission, 0.5);
	load_color(material, AI_MATKEY_COLOR_DIFFUSE, diffuse, 0.5);
	load_color(material, AI_MATKEY_COLOR_SPECULAR, specular, 0.5);
	material->Get(AI_MATKEY_SHININESS, shininess);
	
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString path;
		
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			string full_path = dir + "/" + path.data;
			texture = new texture_t(GL_TEXTURE_2D, full_path.c_str());
			
			if (!texture->load()) {
				fprintf(stderr, "Error loading texture '%s'\n", full_path.c_str());
				delete texture;
				texture = NULL;
				res = false;
			} else {
				printf("Loaded texture '%s'\n", full_path.c_str());
			}
		}
	}
	
	// Load a white texture in case the model does not include its own texture
	if (!texture) {
		printf("loading white\n");
		texture = new texture_t(GL_TEXTURE_2D, "./models/white.png");
		
		res = texture->load();
	}
	
	if (texture) {
		texture->init_bind();
		texture_size = texture->size();
	} else {
		texture_size = 0;
	}
	
	return res;
}

void material_t::clear() {
	if (texture) {
		delete texture;
		texture = NULL;
	}
	texture_size = 0;
}

material_t::~material_t() {
	clear();
}

void material_t::apply() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	if (texture) {
		texture->bind();
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void material_t::draw_associated_meshes(glm::mat4 V) {
	if (meshes.size()) {
		apply();
		for (unsigned i = 0, ilen = meshes.size(); i < ilen; ++i) {
			meshes[i]->draw(true, false, V);
		}
	}
}

string material_t::get_texture_file_name() {
	return texture->get_file_name();
}

int material_t::compare_textures(const material_t &o) const {
	if (texture == o.texture) {
		return 0;
	} else if (texture != NULL && o.texture != NULL) {
		return texture->get_file_name().compare(o.texture->get_file_name());
	} else {
		return texture - o.texture;
	}
}

bool material_t::operator<(const material_t &o) const {
	return compare_textures(o) < 0;
}

bool material_t::operator==(const material_t &o) const {
	return (compare_textures(o) == 0)
			&& !(memcmp(ambient, o.ambient, 4)
			|| memcmp(emission, o.emission, 4)
			|| memcmp(diffuse, o.diffuse, 4)
			|| memcmp(specular, o.specular, 4)
			|| shininess != o.shininess);
}

void material_t::substitute(material_t *o) {
	unsigned ilen = meshes.size();
	o->meshes.reserve(o->meshes.size() + ilen);
	for (unsigned i = 0; i < ilen; ++i) {
		meshes[i]->set_material(o);
		o->meshes.push_back(meshes[i]);
	}
}
