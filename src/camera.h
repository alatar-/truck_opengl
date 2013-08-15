#ifndef CAMERA_H
#define CAMERA_H
#include <stdlib.h>
#include <cmath>

#include "shared.h"
#include "vertex.h"

#define CAMERA_RADIUS 100
const float EPSILON =  1/10e9;

class World;
#include "world.h"

class Camera {
private:
	vertex_3d<float>position;
	float angle_horizontal, angle_vertical;
    float max_vertical_angle_up, max_vertical_angle_down;
    float speed;
	int time_last;

    int animation_on;
    int animation_time_last;
    float animation_angle_vertical_deviation, animation_angle_horizontal_deviation;
    float animation_angle_vertical_goto, animation_angle_horizontal_goto;
    int animation_time_duration;
    float animation_rand_range; // const
    int animation_time_estimated; // const
    float animation_speed; // const
public:
    Camera(float position_x, float position_y, float position_z, float angle_horizontal, float speed, float in_max_vertical_angle_up, float in_max_vertical_angle_down, float animation_rand_range, int animation_time_estimated, float animation_speed);
    void mouse_motion(float angle_horizontal_delta, float angle_vertical_delta);
	void move(direct_t right_left, direct_t front_back, direct_t up_down);
	glm::mat4 get_view_matrix();
    void animate_crash();
    void tmp_animate() {
        if (!animation_on) {
            animation_on = 1;
        }
    }
};

#endif
