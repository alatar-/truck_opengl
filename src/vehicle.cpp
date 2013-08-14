#include "vehicle.h"

Vehicle::Vehicle(float in_size,
    float in_max_acceleration,
    float in_max_velocity,
    float in_max_reverse_velocity,
    float in_turn_acceleration,
    float in_min_turn,
    float in_max_turn,
    float in_begining_turn_point,
    float in_max_turn_point,
    float in_final_turn_point,
    float in_max_following_bend,
    float in_time_following_bend
    // float max_wheel,
    // float in_rotate_time
) : Rectangle() {
	following_vehicle = NULL;
    this->left_steering_wheel = NULL;
    this->right_steering_wheel = NULL;

    size = in_size;

    max_acceleration = in_max_acceleration;
    max_velocity = in_max_velocity;
    max_reverse_velocity = in_max_reverse_velocity;

    turn_acceleration = in_turn_acceleration;
    min_turn = in_min_turn;
    max_turn = in_max_turn;

    begining_turn_point = in_begining_turn_point;
    max_turn_point = in_max_turn_point;
    final_turn_point = in_final_turn_point;

    max_following_bend = in_max_following_bend;
    time_following_bend = in_time_following_bend;
    current_max_following_bend = max_following_bend;
    // max_wheel_rotating_time = max_wheel;

    velocity = 0.0;
    angle = 0;
    following_bend = 0;
    position = vertex_2d(0, 0);
}

float Vehicle::acceleration(direct_t front_back, float velocity) {
    if (front_back == FORW) {
        if (velocity <= 0) {
            return max_acceleration;
        } else {
            return max_acceleration * (1 -(velocity / max_velocity));
        }
    } else if (front_back == BACK) {
        if (velocity < 0) {
            return max_acceleration * ((velocity / -max_reverse_velocity) - 1);
        } else {
            return -max_acceleration;
        }
    } else {
        return -sign(velocity) * max_acceleration;
    }
}

float Vehicle::bend_acceleration(direct_t right_left, float in_following_bend, float in_velocity) {
    if (in_velocity > 0) {
        in_velocity /= max_velocity;
        if (right_left && abs(in_following_bend) <= current_max_following_bend ) {
            return right_left * max_following_bend * in_velocity;
        } else {
            return -sign(in_following_bend) * max_following_bend * in_velocity;
        }
    } else {
        return 0;
    }
}

float Vehicle::turn_factor() {
    float norm_x = abs(velocity) / max_velocity - begining_turn_point;
    float x = norm_x * PI;
    float y;

    if ( norm_x > 0 && norm_x < (final_turn_point + 1) * max_turn_point ) {
        if ( norm_x < max_turn_point ) {
            y = sin( x / (2 * max_turn_point));
        } else {
            y = cos( (x - max_turn_point * PI) / (2 * (1 - max_turn_point)));
        }
    } else {
        y = 0;
    }

    return min_turn + y * (max_turn - min_turn);
}

void Vehicle::calculate(direct_t front_back, direct_t right_left) {
    int now = glutGet(GLUT_ELAPSED_TIME); 
    float dt = ((float)now - last_time) / 1000.0;
    last_time = now;

    float new_velocity = velocity + acceleration(front_back, velocity) * dt;
    if ((front_back == STOP) && (sign(new_velocity) * sign(velocity) < 0)) {
        velocity = 0;
    } else {
        velocity = min(max(new_velocity, -max_reverse_velocity), max_velocity);
    }

    float new_turn_factor = turn_factor();

    if (velocity != 0.0) {
        float new_following_bend = following_bend + bend_acceleration(right_left, following_bend, velocity) * dt / time_following_bend;
        if ((right_left == STOP) && (sign(new_following_bend) * sign(following_bend) < 0)) {
            following_bend = 0;
        } else {
            float new_max_bend = min(new_turn_factor, max_following_bend);
            if (new_following_bend < -current_max_following_bend && following_bend >= -current_max_following_bend) {
                following_bend = -current_max_following_bend;
            } else if (new_following_bend > current_max_following_bend && following_bend <= current_max_following_bend) {
                following_bend = current_max_following_bend;
            } else {
                following_bend = new_following_bend;
            }
            current_max_following_bend = new_max_bend;
        }
    }

    if (velocity != 0 && right_left != STOP) {
        angle = normalize_angle(angle - sign(velocity) * right_left * dt * new_turn_factor);
    }

    float ds = -velocity * dt;
    position.x += ds * sin(angle);
    position.y += ds * cos(angle);

    move(0, position, angle, ds, following_bend);
}

// TODO
// MAX_WHEEL_ROTATING_TIME

void Vehicle::move(float parent_size, vertex_2d in_position, float in_angle, float ds, float in_following_bend) {
    if (parent_size != 0) {
        vertex_2d d_pos(0, size / 2);
        d_pos = d_pos.rotate(-in_following_bend);
        d_pos.y += parent_size;
        d_pos = d_pos.rotate(-in_angle);
        position = in_position + d_pos;

        angle = in_angle + in_following_bend;
    } else {
        if (left_steering_wheel) {
            left_steering_wheel->move(position, angle, ds);
            left_steering_wheel->rotate(-in_following_bend);
            right_steering_wheel->move(position, angle, -ds);
            right_steering_wheel->rotate(-in_following_bend);
        }
    }
    body->move(position, angle, 0);
    for (unsigned i = 0; i < this->left_wheels.size(); ++i) {
        left_wheels[i]->move(position, angle, ds);
        right_wheels[i]->move(position, angle, -ds);
    }
    if (following_vehicle) {
        following_vehicle->move(size, position, angle, ds, in_following_bend);
    }
}

Vehicle::~Vehicle() {
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}

vector <vertex_2d>  Vehicle::get_body_vertices() {
    dimensions.clear();
    vertex_2d pos_min = body->get_model_min_point();
    printf("vertex: ( %f ; %f ) \n", pos_min.x, pos_min.y);
    vertex_2d pos_max = body->get_model_max_point();
    dimensions.push_back(pos_min);
    float tmp = pos_min.y;
    pos_min.y = pos_max.y;
    printf("vertex: ( %f ; %f ) \n", pos_min.x, pos_min.y);
    dimensions.push_back(pos_min);
    printf("vertex: ( %f ; %f ) \n", pos_max.x, pos_max.y);
    dimensions.push_back(pos_max);
    pos_max.y = tmp;
    printf("vertex: ( %f ; %f ) \n", pos_max.x, pos_max.y);
    dimensions.push_back(pos_max);
    return dimensions;
}
