#include "texture.h"

#include <cstdio>

Texture::Texture(GLenum in_texture_target, const string& in_file_name) : file_name(in_file_name) {
	texture_target = in_texture_target;
	image = NULL;
}

bool Texture::load() {
	try {
		image = new Magick::Image(file_name);
		image->write(&blob, "RGBA");
	} catch (Magick::Error& error) {
		fprintf(stderr, "Error while loading texture '%s': %s\n", file_name.c_str(), error.what());
		return false;
	}
	
	return true;
}

void Texture::init_bind() {
	glGenTextures(1, &texture_obj);
	glBindTexture(texture_target, texture_obj);
	glTexImage2D(texture_target, 0, GL_RGB, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind() {
	glBindTexture(texture_target, texture_obj);
// 	printf("Binding (%p): '%s'\n", this, file_name.c_str());
}

double Texture::size() {
	if (this && image) {
		return (double)image->columns() * image->rows();
	} else {
		return 0;
	}
}

string Texture::get_file_name() {
	return file_name;
}

bool Texture::operator==(const Texture &o) const {
	return file_name == o.file_name;
}
