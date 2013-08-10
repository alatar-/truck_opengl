#include "light.h"


glm::mat4 light_t::bias = glm::mat4(
		glm::vec4(0.5f, 0.0f, 0.0f, 0.0f)
	,	glm::vec4(0.0f, 0.5f, 0.0f, 0.0f)
	,	glm::vec4(0.0f, 0.0f, 0.5f, 0.0f)
	,	glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

light_t::light_t (glm::vec3 in_pos, glm::vec3 look_at, unsigned in_slot) {
	P = glm::perspective(45.0f, 1.0f, 2.0f, 8.0f);
	V = glm::lookAt(in_pos, look_at, glm::vec3(0.0f, 1.0f, 0.0f));
	pos = glm::vec4(in_pos, 1.0f);
	slot = GL_LIGHT0 + in_slot;
}

glm::vec4 light_t::get_position() {
	return pos;
}

void light_t::activate() {
	glLightfv(slot, GL_POSITION, &pos.x);
	glEnable(slot);
}

void light_t::deactivate() {
	glDisable(slot);
}

void light_t::set_dim_light() {
	float dim[] = { 0.2f, 0.2f, 0.2f }
		,	black[] = { 0.0f, 0.0f, 0.0f };
	glLightfv(slot, GL_AMBIENT, dim);
	glLightfv(slot, GL_DIFFUSE, dim);
	glLightfv(slot, GL_SPECULAR, black);
}

void light_t::set_bright_light() {
	float white[] = { 1.0f, 1.0f, 1.0f }
		,	black[] = { 0.0f, 0.0f, 0.0f };
	glLightfv(slot, GL_DIFFUSE, white);
	glLightfv(slot, GL_SPECULAR, white);
	glLightfv(slot, GL_SPECULAR, black);
}

glm::mat4 light_t::get_tex_m() {
	return light_t::bias * P * V;
}

glm::mat4 light_t::get_P() {
	return P;
}

glm::mat4 light_t::get_V() {
	return V;
}
