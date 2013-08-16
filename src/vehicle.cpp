#include "vehicle.h"

Vehicle::Vehicle(World *world,
    float in_size,
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
    float in_time_following_bend,
    float in_time_side_velocity,
    float in_minimal_delta_side_velocity,
    float in_max_side_velocity,
    float in_reflection_angle_fraction,
    float in_reflection_velocity_fraction,
    float in_min_reflection
    // float max_wheel,
    // float in_rotate_time
) : Rectangle() {
    this->world = world;
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


    time_side_velocity = in_time_side_velocity;
    minimal_delta_side_velocity = in_minimal_delta_side_velocity;
    max_side_velocity = in_max_side_velocity;

    reflection_angle_fraction = in_reflection_angle_fraction;
    reflection_velocity_fraction = in_reflection_velocity_fraction;
    min_reflection = in_min_reflection;
    // max_wheel_rotating_time = max_wheel;

    velocity = 0.0;
    side_velocity = 0.0;
    angle = 0;
    following_bend = 0;
    position = Vertex2D<float>(0, 0);
}

float Vehicle::acceleration(direct_t front_back, float velocity) {
    if (front_back == FRONT) {
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

    if (side_velocity != 0.0) {
        float delta_side_velocity = -sign(side_velocity) * max(minimal_delta_side_velocity, abs(side_velocity) * dt / time_side_velocity);
        float new_side_velocity = side_velocity + delta_side_velocity;
        if (sign(new_side_velocity) * sign(side_velocity) < 0) {
            side_velocity = 0.0;
        } else {
            angle += atan2(delta_side_velocity, velocity);
            velocity = module<float>(velocity, delta_side_velocity);
            side_velocity = new_side_velocity;
        }
    }

    if (velocity != 0 && right_left != STOP) {
        angle = normalize_angle(angle - sign(velocity) * right_left * dt * new_turn_factor);
    }

    float ds = -velocity * dt;
    Vertex2D<float>new_position(position.x + ds * sin(angle), position.y + ds * cos(angle));

    // printf("\nNew movement:\n");
    // position.print();
    // new_position.print();
    move(0, new_position, angle, ds, following_bend, obstacles, dt);
}

bool Vehicle::move(float parent_size, Vertex2D<float>in_position, float in_angle, float ds, float in_following_bend, vector <Obstacle*> &obstacles, float dt) {
    if (parent_size != 0) {
        Vertex2D<float>d_pos(0, size / 2);
        d_pos = d_pos.rotate(-in_following_bend);
        d_pos.y += parent_size;
        d_pos = d_pos.rotate(-in_angle);
        in_position += d_pos;

        angle = in_angle + in_following_bend;
    }

    Vertex2D <float> delta_pos = detect_collision(obstacles, in_position, dt);
    bool res = delta_pos.is_blank();
    if (res && (!following_vehicle || following_vehicle->move(size, in_position, angle, ds, in_following_bend, obstacles, dt))) {
        position = in_position + delta_pos;
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
    } else {
        position += delta_pos;
    }
    set_vertices();
    return res;
}

Vehicle::~Vehicle() {
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}

vector <Vertex2D<float> > Vehicle::get_body_vertices() {
    // printf("\n%p> Vehicle::get_body_vertices\n", this);

    dimensions.clear();
    Vertex2D<float>pos_min = body->get_model_min_point();
    // printf("vertex: ( %f , %f ) \n", pos_min.x, pos_min.y);
    Vertex2D<float>pos_max = body->get_model_max_point();
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
    vector <Vertex2D<float> > verts = get_body_vertices();

    for (unsigned i = 0; i < 4; ++i) {
        verts[i] = verts[i].rotate(-angle) + position;
    }

    Rectangle::set_vertices(verts);
}


Vertex2D <float> Vehicle::detect_collision(vector <Obstacle*> &obstacles, Vertex2D<float> &in_position, float dt) {
    Vertex2D <float> coll_face_norm;
    Rectangle rect;
    Rectangle in_rect;

    {
        rect = *((Rectangle*)this);

        Vertex2D <float> temp_position = position;
        position = in_position;

        set_vertices();
        in_rect = *((Rectangle*)this);

        position = temp_position;
    }

    for(unsigned i = 0; i < obstacles.size(); ++i) {
        if(in_rect.intersection(*obstacles[i], coll_face_norm)) {
            if(rect.intersection(*obstacles[i], coll_face_norm)){
                coll_face_norm = (-sign(velocity) * sin(angle), -sign(velocity) * cos(angle));
            }

            printf("collision with obstacle %d\n", i);
            // obstacles[i]->print();
            // printf("vehicle position:\n");
            // print();
            return collision(coll_face_norm, dt);
        }
    }
    return Vertex2D<float>();
}

Vertex2D <float> Vehicle::collision(Vertex2D <float> coll_face_norm, float dt) {
    // printf("\n\n\nCOLLISION\n\nfrom:\n");
    // coll_face_norm.print();
    coll_face_norm = coll_face_norm.normal();
    // printf("to:\n");
    // coll_face_norm.print();

    Vertex2D <float> v(velocity * sin(angle), velocity * cos(angle)); // not sure
    // printf("v(point):\n");
    // v.print();

    float alpha = v.angle_between(coll_face_norm);
    // printf("angle_between = %f\n", alpha);
    angle -= 2 * alpha * reflection_angle_fraction;
    // printf("angle = %f, sin = %f, cos = %f\n", angle, sin(angle), cos(angle));

    velocity *= reflection_velocity_fraction;

    // printf("v = %f\n", velocity);

    v = Vertex2D <float>(velocity * sin(angle), velocity * cos(angle));

    // printf("v(point again):\n");
    // v.print();

    alpha = v.angle_between(coll_face_norm);

    float cs = cos(alpha);
    float sn = sin(alpha);
    float v_tan = v * cs;
    side_velocity = max<float>(min<float>((float)(side_velocity + v_tan * sn), (float)max_side_velocity), -(float)max_side_velocity);
    velocity = v.module() + v_tan * cs;


    // printf("position:\n");
    // position.print();
    // printf("v = %f, s_v = %f, ang = %f:, v_tan = %f\n", velocity, side_velocity, angle, v_tan);

    world->tmp_animate();
    // printf("KOLIZJA!\n");
    return coll_face_norm * -min_reflection * dt;
}

void Vehicle::attach(Vehicle *following) {
    following_vehicle = following;


    following->max_acceleration = max_acceleration;
    following->max_velocity = max_velocity;
    following->max_reverse_velocity = max_reverse_velocity;

    following->turn_acceleration = turn_acceleration;
    following->min_turn = min_turn;
    following->max_turn = max_turn;

    following->begining_turn_point = begining_turn_point;
    following->max_turn_point = max_turn_point;
    following->final_turn_point = final_turn_point;

    following->max_following_bend = max_following_bend;
    following->time_following_bend = time_following_bend;
    following->current_max_following_bend = max_following_bend;


    following->time_side_velocity = time_side_velocity;
    following->minimal_delta_side_velocity = minimal_delta_side_velocity;

    following->reflection_angle_fraction = reflection_angle_fraction;
    following->reflection_velocity_fraction = reflection_velocity_fraction;
    following->min_reflection = min_reflection;
}
