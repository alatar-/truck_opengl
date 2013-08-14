#ifndef VEHICLE_H
#define VEHICLE_H

#include "truck_part.h"

class Vehicle {
private:
    float max_acceleration;
    float max_velocity;
    float max_reverse_velocity;
    float turn_acceleration;
    float min_turn;
    float max_turn;
    // int max_wheel_rotating_time;
    // float max_angle;
    float acceleration(direct_t front_back, float velocity);
    float turn_factor();
public:
  vector <TruckPart*> left_wheels;
  vector <TruckPart*> right_wheels;
  TruckPart* left_steering_wheel;
  TruckPart* right_steering_wheel;
  TruckPart* body;
  Vehicle* following_vehicle;

    float velocity;
    float angle;
    float size;
    vertex_2d position;
    int last_time;

	
	Vehicle(float in_size,
    float in_max_acceleration,
    float in_max_velocity,
    float in_max_reverse_velocity,
    float in_turn_acceleration,
    float in_min_turn,
    float in_max_turn
    );
 
	~Vehicle();


    void calculate(direct_t front_back, direct_t right_left);
    void move(float parent_size, vertex_2d position, float angle, float ds);
	// void set_mv_matrix(glm::mat4 V);
	// void apply_mv();
};

#endif