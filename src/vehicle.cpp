#include "vehicle.h"

Vehicle::Vehicle(float in_size,
    float in_max_acceleration,
    float in_max_velocity,
    float in_max_reverse_velocity,
    float in_turn_acceleration,
    float in_min_turn,
    float in_max_turn,
    float in_max_following_bend,
    float in_time_following_bend
    // float max_wheel,
    // float in_rotate_time
) {
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

    max_following_bend = in_max_following_bend;
    time_following_bend = in_time_following_bend;
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
        if (right_left) {
            return right_left * max_following_bend * in_velocity;
        } else {
            return -sign(in_following_bend) * max_following_bend * in_velocity;
        }
    } else {
        return 0;
    }
}

float Vehicle::turn_factor() {
    return min(max(turn_acceleration / abs(velocity), min_turn), max_turn);
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

    if (velocity != 0.0) {
        float new_following_bend = following_bend + bend_acceleration(right_left, following_bend, velocity) * dt / time_following_bend;
        if ((right_left == STOP) && (sign(new_following_bend) * sign(following_bend) < 0)) {
            following_bend = 0;
        } else {
            if (new_following_bend < -max_following_bend && following_bend >= -max_following_bend) {
                following_bend = -max_following_bend;
            } else if (new_following_bend > max_following_bend && following_bend <= max_following_bend) {
                following_bend = max_following_bend;
            } else {
                following_bend = new_following_bend;
            }
        }
    }

    if (velocity != 0 && right_left != STOP) {
        angle = normalize_angle(angle - sign(velocity) * right_left * dt * turn_factor());
    }

    float ds = -velocity * dt;
    position.x += ds * sin(angle);
    position.y += ds * cos(angle);

    move(0, position, angle, ds, following_bend);
}

// TODO: FRONT WHEEL TURN
// MAX_WHEEL_ROTATING_TIME

void Vehicle::move(float parent_size, vertex_2d in_position, float in_angle, float ds, float in_following_bend) {
    if (parent_size != 0) {
        vertex_2d d_pos(0, size / 2);
        d_pos = d_pos.rotate(-in_following_bend);
        d_pos.y += parent_size;
        d_pos = d_pos.rotate(-in_angle);
        position = in_position + d_pos;

        angle = in_angle + in_following_bend;
    }

    body->move(position, angle, 0);
    for (unsigned i = 0; i < this->left_wheels.size(); ++i) {
        left_wheels[i]->move(position, angle, ds);
        right_wheels[i]->move(position, angle, -ds);
    }
    if(left_steering_wheel) {
        left_steering_wheel->move(position, angle, ds);
        // left_steering_wheel->rotate((direct_t)(-X));
    }
    if(right_steering_wheel) {
        right_steering_wheel->move(position, angle, -ds);
        // right_steering_wheel->rotate(X);
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
