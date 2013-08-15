#include "camera.h"
#include <math.h>

#include "shared.h"

Camera::Camera(float position_x, float position_y, float position_z, float angle_horizontal, float speed, float in_max_vertical_angle_up, float in_max_vertical_angle_down) : position(position_x, position_y, position_z) {
	this->angle_horizontal = angle_horizontal;
	this->angle_vertical = 0;

    max_vertical_angle_up = PI / 2 * in_max_vertical_angle_up;
    max_vertical_angle_down = PI / 2 * in_max_vertical_angle_down;

    this->speed = speed / 1000;
	this->time_last = glutGet(GLUT_ELAPSED_TIME);
}

void Camera::mouse_motion(float angle_horizontal_delta, float angle_vertical_delta) {
	angle_horizontal = normalize_angle(angle_horizontal - angle_horizontal_delta);
    angle_vertical = max(min(angle_vertical - angle_vertical_delta, max_vertical_angle_up), -max_vertical_angle_down);
}

void Camera::move(direct_t right_left, direct_t front_back, direct_t up_down) {
	int time_now = glutGet(GLUT_ELAPSED_TIME);
    int time_delta = time_now - time_last;
	time_last = time_now;

	float speed_delta = speed * time_delta;
	float sinus = sin(angle_horizontal);
    float cosinus = cos(angle_horizontal);

	position = vertex_3d(
        position.x + speed_delta * (front_back  * sinus + right_left * cosinus),
        position.y + speed_delta * (front_back * cosinus - right_left * sinus),
        position.z + speed_delta * up_down
    );
}

glm::mat4 Camera::get_view_matrix() {
    float xy = CAMERA_RADIUS * cos(angle_vertical);
    float o_xz = -sin(angle_vertical);
    float sin_h = sin(angle_horizontal);
    float cos_h = cos(angle_horizontal);
	return glm::lookAt(
		glm::vec3(position.x, position.z, position.y),
		glm::vec3(position.x + xy * sin_h, position.z + CAMERA_RADIUS * sin(angle_vertical), position.y + xy * cos_h),
		glm::vec3(o_xz * sin_h, cos(angle_vertical), o_xz * cos_h)
	);
}