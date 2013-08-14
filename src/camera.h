#ifndef CAMERA_H
#define CAMERA_H

#include "shared.h"
#include "vertex.h"

#define CAMERA_RADIUS 100

class world_t;
#include "world.h"

class Camera {
private:
	vertex_3d position;
	float angle_horizontal, angle_vertical;
  float max_vertical_angle_up, max_vertical_angle_down;
    float speed;
	int time_last;
public:
    Camera(float position_x, float position_y, float position_z, float angle_horizontal, float speed, float in_max_vertical_angle_up, float in_max_vertical_angle_down);
    void mouse_motion(float angle_horizontal_delta, float angle_vertical_delta);
	void move(direct_t right_left, direct_t front_back, direct_t up_down);
	glm::mat4 get_view_matrix();
};

#endif
