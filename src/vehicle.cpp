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
    position = vertex_2d<float>(0, 0);
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

void Vehicle::calculate(direct_t front_back, direct_t right_left, vector <Obstacle*> &obstacles) {
    int now = glutGet(GLUT_ELAPSED_TIME); 
    float dt = ((float)now - last_time) / 1000.0;
    last_time = now;
    // printf("FPS: %f\n", 1 / dt);
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
    vertex_2d<float>new_position(position.x + ds * sin(angle), position.y + ds * cos(angle));

    printf("\nNew movement:\n");
    position.print();
    new_position.print();
    if(!move(0, new_position, angle, ds, following_bend, obstacles)) {
        velocity = 0;
    }
}

bool Vehicle::move(float parent_size, vertex_2d<float>in_position, float in_angle, float ds, float in_following_bend, vector <Obstacle*> &obstacles) {
    if (parent_size != 0) {
        vertex_2d<float>d_pos(0, size / 2);
        d_pos = d_pos.rotate(-in_following_bend);
        d_pos.y += parent_size;
        d_pos = d_pos.rotate(-in_angle);
        in_position += d_pos;

        angle = in_angle + in_following_bend;
    }

    vertex_2d<float>temp_position = position;
    position = in_position;
    set_vertices();
    position = temp_position;
    if (!detect_collision(obstacles)) {
        if (!following_vehicle || following_vehicle->move(size, in_position, angle, ds, in_following_bend, obstacles)) {
            position = in_position;
            body->move(in_position, angle, 0);
            if (left_steering_wheel && right_steering_wheel) {
                left_steering_wheel->move(in_position, angle, ds);
                left_steering_wheel->rotate(-in_following_bend);
                right_steering_wheel->move(in_position, angle, -ds);
                right_steering_wheel->rotate(-in_following_bend);
            }

            for (unsigned i = 0; i < this->left_wheels.size(); ++i) {
                left_wheels[i]->move(in_position, angle, ds);
                right_wheels[i]->move(in_position, angle, -ds);
            }
        }
        set_vertices();
        return true;
    } else {
        collision();
        set_vertices();
        return false;
    }
}

Vehicle::~Vehicle() {
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}

vector <vertex_2d<float> > Vehicle::get_body_vertices() {
    // printf("\n%p> Vehicle::get_body_vertices\n", this);

    dimensions.clear();
    vertex_2d<float>pos_min = body->get_model_min_point();
    // printf("vertex: ( %f , %f ) \n", pos_min.x, pos_min.y);
    vertex_2d<float>pos_max = body->get_model_max_point();
    dimensions.push_back(pos_min);
    float tmp = pos_min.y;
    pos_min.y = pos_max.y;
    // printf("vertex: ( %f , %f ) \n", pos_min.x, pos_min.y);
    dimensions.push_back(pos_min);
    pos_min.y = tmp;
    pos_min.x = pos_max.x;
    // printf("vertex: ( %f , %f ) \n", pos_min.x, pos_min.y);
    dimensions.push_back(pos_min);
    // printf("vertex: ( %f , %f ) \n", pos_max.x, pos_max.y);
    dimensions.push_back(pos_max);
    return dimensions;
}

void Vehicle::set_vertices() {
    vector <vertex_2d<float> > verts = get_body_vertices();

    for (unsigned i = 0; i < 4; ++i) {
        verts[i] = verts[i].rotate(-angle) + position;
    }

    Rectangle::set_vertices(verts);
}


bool Vehicle::detect_collision(vector <Obstacle*> &obstacles) {
    for(unsigned i = 0; i < obstacles.size(); ++i) {
        if(intersection(*obstacles[i])) {
            printf("collision with obstacle %d\n", i);
            obstacles[i]->print();
            printf("vehicle position:\n");
            print();
            return true;
        }
    }
    if (following_vehicle) {
        return following_vehicle->detect_collision(obstacles);
    }
    return false;
}

void Vehicle::collision() {
    printf("KOLIZJA!\n");
}
