#ifndef _INCLUDE_TEXTURE_H
#define _INCLUDE_TEXTURE_H

#include <string>
#include "shared.h"

using namespace std;

class texture_t {
private:
	string file_name;
	GLenum texture_target;
	GLuint texture_obj;
	Magick::Image* image;
	Magick::Blob blob;
public:
	texture_t(GLenum in_texture_target, const string& in_file_name);
	void init_bind();

	string get_file_name();

	bool load();
	void bind();
	double size();

	bool operator==(const texture_t &o) const;
};


#endif
