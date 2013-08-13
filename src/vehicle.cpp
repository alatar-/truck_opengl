#include "vehicle.h"

Vehicle::Vehicle(
    float max_acc,
    int max_wheel,
    int rotate
) {
	following_vehicle = NULL;
    this->left_steering_wheel = NULL;
    this->right_steering_wheel = NULL;

    max_acceleration = max_acc;
    max_wheel_rotating_time = max_wheel;
    rotate_time = rotate;

    velocity = 0.0;
    angle = 0;
    position = vertex_2d(0, 0);
}

float acceleration(direct_t front_back, float velocity) {
    if (front_back == FORW) {
        if (velocity <= 0) {
            return MAX_ACC;
        } else {
            return MAX_ACC * (1 -(velocity / MAX_VELOCITY));
        }
    } else if (front_back == BACK) {
        if (velocity < 0) {
            return MAX_ACC * ((velocity / MAX_REVERSE_VELOCITY) - 1)
        } else {
            return -MAX_ACC;
        }
    } else {
        return -sign(velocity) * MAX_ACC;
    }
}

void Vehicle::calculate(direct_t front_back, direct_t right_left) {
    int now = glutGet(GLUT_ELAPSED_TIME); 
    int dt = now - last_time;
    last_time = now;

    float new_velocity = velocity + acceleration(front_back, velocity) * dt;

    delta_angle = dt * (max_wheel_rotating_time / );
    angle = min(angle + delta_angle, max_angle)


    // if(speed <= 60)
    // {
    //  speed ++;
    //  printf("speed checking, X = %d, speed = %d\n", X, speed);
    // }
    float fl_speed = (float) speed/1000;
    set_pos(pos.x, pos.y - (fl_speed * dt) * X);    

    this->move(position, angle, velocity * dt)
}

void Vehicle:move(vertex_2d position, float angle, float ds) {
    // this->body->move(X);
    // for (unsigned i = 0; i < this->left_wheels.size(); ++i) {
    //     this->left_wheels[i]->move(X);
    //     this->right_wheels[i]->move(X);
    //     this->left_wheels[i]->rotate((direct_t)(-X));
    //     this->right_wheels[i]->rotate(X);
    // }
    // if(this->left_steering_wheel) {
    //     this->left_steering_wheel->move(X);
    //     this->left_steering_wheel->rotate((direct_t)(-X));
    // }
    // if(this->right_steering_wheel) {
    //     this->right_steering_wheel->move(X);
    //     this->right_steering_wheel->rotate(X);
    // }
    // if (this->following_vehicle) {
    //     this->following_vehicle->move(X);
    // }
}

Vehicle::~Vehicle() {
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}
