#include "texture.h"

#include <cstdio>

texture_t::texture_t(GLenum in_texture_target, const string& in_file_name) {
	texture_target = in_texture_target;
	file_name = in_file_name;
	image = NULL;
}

bool texture_t::load() {
	try {
		image = new Magick::Image(file_name);
		image->write(&blob, "RGBA");
	} catch (Magick::Error& error) {
		fprintf(stderr, "Error while loading texture '%s': %s\n", file_name.c_str(), error.what());
		return false;
	}
	
	return true;
}

void texture_t::init_bind() {
	glGenTextures(1, &texture_obj);
	glBindTexture(texture_target, texture_obj);
	glTexImage2D(texture_target, 0, GL_RGB, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void texture_t::bind() {
	glBindTexture(texture_target, texture_obj);
// 	printf("Binding (%p): '%s'\n", this, file_name.c_str());
}

double texture_t::size() {
	if (this && image) {
		return (double)image->columns() * image->rows();
	} else {
		return 0;
	}
}
