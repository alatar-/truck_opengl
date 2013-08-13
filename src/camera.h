#ifndef _INCLUDE_PLAYER_H
#define _INCLUDE_PLAYER_H

#include "shared.h"
#include "vertex.h"

class world_t;
#include "world.h"

class player_t {
private:
	vertex_3d pos;
	float ang_h
		,	ang_v
		,	height
		,	speed
		,	size;
	int last_time;
	world_t *world;
public:
	player_t (world_t *world, float in_pos_x, float in_pos_y, float in_ang_h, float in_height, float in_speed, float in_size);
	void mouse_motion (float dang_h, float dang_v);
	void move (direct_t keys_h, direct_t keys_v, direct_t height);
	vertex_2d get_pos();
	float get_size();
	
	glm::mat4 get_view_matrix();
};

#endif
