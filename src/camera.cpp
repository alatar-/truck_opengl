#include "camera.h"
#include <math.h>

#include "shared.h"

Camera::Camera(float position_x, float position_y, float position_z, float angle_horizontal, float speed) : position(position_x, position_y, position_z) {
	this->angle_horizontal = angle_horizontal;
	this->angle_vertical = 0;

    this->speed = speed / 1000;
	this->time_last = glutGet(GLUT_ELAPSED_TIME);
}

void Camera::mouse_motion(float angle_horizontal_delta, float angle_vertical_delta) {
	angle_horizontal -= angle_horizontal_delta;
	angle_vertical -= angle_vertical_delta;
	if (angle_vertical < -PI / 2) {
		angle_vertical = -PI / 2;
	} else if (angle_vertical > PI / 2) {
	   angle_vertical = PI / 2;
	}
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
	return glm::lookAt(
		glm::vec3(position.x, position.z, position.y),
		glm::vec3(position.x + 10 * sin(angle_horizontal), position.z + 10 * sin(angle_vertical), position.y + 10 * cos(angle_horizontal)),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
}