#include "player.h"
#include <math.h>

#include "shared.h"

player_t::player_t (world_t *in_world, float in_pos_x, float in_pos_y, float in_ang_h, float in_height, float in_speed, float in_size) : pos(in_pos_x, in_pos_y, in_height) {
	world = in_world;
	
	ang_h = in_ang_h;
	ang_v = 0;
	speed = in_speed / 1000;
	size = in_size;
	
	height = in_height;
	last_time = glutGet(GLUT_ELAPSED_TIME);
}

void player_t::mouse_motion (float dang_h, float dang_v) {
	ang_h -= dang_h;
	ang_v -= dang_v;
	if (ang_v < -PI / 2) {
		ang_v = -PI / 2;
	} else if (ang_v > PI / 2) {
		ang_v = PI / 2;
	}
}

void player_t::move (direct_t keys_h, direct_t keys_v, direct_t height) {
	int now = glutGet(GLUT_ELAPSED_TIME)
		,	dt = now - last_time;
	last_time = now;
	float dspeed = speed * dt
		,	sn = sin(ang_h)
		,	cs = cos(ang_h);
	//printf("height: %d, position: %f\n", height, pos.z);
	vertex_3d itd(pos.x + dspeed * (keys_v  * sn + keys_h * cs)
		,	pos.y + dspeed * (keys_v * cs - keys_h * sn) 
		,	pos.z + dspeed * height
		);
	pos = itd;
	// if (world->test_colls_with_galery(pos, itd, size, height)) {
	// 	world->lock();
	// 			pos = itd;
	// 	world->unlock();
	// }
}

glm::mat4 player_t::get_view_matrix() {
	return glm::lookAt(
		glm::vec3(pos.x, pos.z, pos.y)
	,	glm::vec3(pos.x + 10 * sin(ang_h), pos.z + 10 * sin(ang_v), pos.y + 10 * cos(ang_h))
	,	glm::vec3(0.0f, 1.0f, 0.0f)
	);
}

vertex_2d player_t::get_pos() {
	return pos;
}

float player_t::get_size() {
	return size;
}
