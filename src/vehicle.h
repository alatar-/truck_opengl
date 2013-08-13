#ifndef VEHICLE_H
#define VEHICLE_H

#include "truck_part.h"

class Vehicle {
private:
    float max_acceleration;
    int max_wheel_rotating_time;
    int rotate_time;
    float max_angle;
public:
	vector <TruckPart*> left_wheels;
	vector <TruckPart*> right_wheels;
	TruckPart* left_steering_wheel;
	TruckPart* right_steering_wheel;
	TruckPart* body;
	Vehicle* following_vehicle;

    float velocity;
    float angle;
    vertex_2d position;
    int last_time;
	
	Vehicle();
	~Vehicle();

    void calculate(direct_t front_back, direct_t right_left);
    void move();
	// void set_mv_matrix(glm::mat4 V);
	// void apply_mv();
};

#endif