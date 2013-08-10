#ifndef _INCLUDE_LIGHT_H
#define _INCLUDE_LIGHT_H

#include "shared.h"

using namespace std;

class light_t {
private:
	glm::mat4 V
		,	P;
	glm::vec4 pos;
	GLenum slot;
	
	static glm::mat4 bias;
public:
	light_t (glm::vec3 pos, glm::vec3 look_at, unsigned in_slot);
	glm::vec4 get_position();
	
	glm::mat4 get_tex_m();
	glm::mat4 get_P();
	glm::mat4 get_V();
	
	void activate();
	void deactivate();
	void set_dim_light();
	void set_bright_light();
};

#endif
