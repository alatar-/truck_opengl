#include "camera.h"
#include <math.h>

#include "shared.h"

Camera::Camera(float position_x, float position_y, float position_z, float angle_horizontal, float speed, float in_max_vertical_angle_up, float in_max_vertical_angle_down, float animation_rand_range, int animation_time_estimated, float animation_speed) : position(position_x, position_y, position_z) {
	this->angle_horizontal = angle_horizontal;
	this->angle_vertical = 0;

    max_vertical_angle_up = PI / 2 * in_max_vertical_angle_up;
    max_vertical_angle_down = PI / 2 * in_max_vertical_angle_down;

    this->speed = speed / 1000;
	this->time_last = glutGet(GLUT_ELAPSED_TIME);

    this->animation_rand_range = animation_rand_range;
    this->animation_time_estimated = animation_time_estimated;
    this->animation_speed = animation_speed;
}

void Camera::animate_crash() {
    if (animation_on) {
        // printf("on "); fflush(stdout);
        int time_now = glutGet(GLUT_ELAPSED_TIME);
        int time_delta = time_now - animation_time_last;
        animation_time_last = time_now;

        if (animation_on == 1) {
            // initialization
            printf("init\n");
            animation_on = 2;
            time_delta = 0; 
            animation_time_duration = 1;
            animation_angle_vertical_deviation = animation_angle_horizontal_deviation = 0;
            animation_angle_vertical_goto = animation_angle_horizontal_goto = 0;
        }
        if (animation_on == 2) {
            // printf("%f %f %f %f\n", animation_angle_horizontal_deviation, animation_angle_vertical_deviation, animation_angle_horizontal_goto, animation_angle_vertical_goto);
            if (animation_time_duration >= animation_time_estimated) {
                printf("in1\n"); fflush(stdout);
                animation_angle_vertical_goto = animation_angle_horizontal_goto = 0;
                if (fabs(animation_angle_horizontal_goto - animation_angle_horizontal_goto) < EPSILON && fabs(animation_angle_vertical_goto - animation_angle_vertical_deviation) < EPSILON) {
                    animation_on = 0;
                } 
            } else if (fabs(animation_angle_horizontal_goto - animation_angle_horizontal_goto) < EPSILON && fabs(animation_angle_vertical_goto - animation_angle_vertical_deviation) < EPSILON) {
                // printf("init %lf %d %lf\n", animation_rand_range, animation_time_estimated, animation_speed);

                int tmp_animation_time_duration = min(2 / (3 * animation_time_estimated), animation_time_duration);
                float rand_range_now = 
                    animation_rand_range * 
                    (1 - tmp_animation_time_duration / 
                    (float)animation_time_estimated);
                printf("in2 %f\n", rand_range_now); fflush(stdout);
                // animation_angle_horizontal_goto = rand() % int(ceil(rand_range_now));
                // animation_angle_vertical_goto = rand() % int(ceil(rand_range_now));
                
                if (rand() % 2) {
                    // animation_angle_horizontal_goto = -animation_angle_horizontal_goto;
                    animation_angle_horizontal_goto = -rand_range_now;
                } else {
                    animation_angle_horizontal_goto = rand_range_now;
                }
                if (rand() % 2) {
                    // animation_angle_vertical_goto = -animation_angle_vertical_goto;
                    animation_angle_vertical_goto = -rand_range_now;
                } else {
                    animation_angle_vertical_goto = rand_range_now;
                }
            }        

            float max_shift = time_delta / (3000 - 100 * animation_speed);
           
            int sign_h;
            if (fabs(animation_angle_horizontal_goto) < EPSILON) {
                sign_h = -sign(animation_angle_horizontal_deviation);
            } else {
                sign_h = sign(animation_angle_horizontal_goto);
            }
            int sign_v;
            if (fabs(animation_angle_vertical_goto) < EPSILON) {
                sign_v = -sign(animation_angle_vertical_deviation);
            } else {
                sign_v = sign(animation_angle_vertical_goto);
            }
            printf("%d %d || %f %f %d | %f %f %d\n", animation_time_duration, animation_time_estimated, animation_angle_horizontal_deviation, 
                animation_angle_horizontal_goto,
                sign_h,
                animation_angle_vertical_deviation, animation_angle_vertical_goto,
                sign_v);
            animation_angle_vertical_deviation = sign_v * min(max_shift, fabs(animation_angle_vertical_goto - animation_angle_vertical_deviation)) + animation_angle_vertical_deviation;
            animation_angle_horizontal_deviation = sign_h * min(max_shift, fabs(animation_angle_horizontal_goto - animation_angle_horizontal_deviation)) + animation_angle_horizontal_deviation;
            printf("%f || %f %f\n", max_shift, animation_angle_horizontal_deviation, animation_angle_vertical_deviation);
            animation_time_duration += time_delta;
        }
    } else {
        // printf("off\n"); fflush(stdout);
    }
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

	position = vertex_3d<float>(
        position.x + speed_delta * (front_back  * sinus + right_left * cosinus),
        position.y + speed_delta * (front_back * cosinus - right_left * sinus),
        position.z + speed_delta * up_down
    );
}

glm::mat4 Camera::get_view_matrix() {
    float ang_v = angle_vertical + animation_angle_vertical_deviation;
    float ang_h = angle_horizontal + animation_angle_horizontal_deviation;
    float xy = CAMERA_RADIUS * cos(ang_v);
    float o_xz = -sin(ang_v);
    float sin_h = sin(ang_h);
    float cos_h = cos(ang_h);
	return glm::lookAt(
		glm::vec3(position.x, position.z, position.y),
		glm::vec3(position.x + xy * sin_h, position.z + CAMERA_RADIUS * sin(ang_v), position.y + xy * cos_h),
		glm::vec3(o_xz * sin_h, cos(ang_v), o_xz * cos_h)
	);
}