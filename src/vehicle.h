#ifndef VEHICLE_H
#define VEHICLE_H

#include "truck_part.h"

class Vehicle {
public:
	vector <TruckPart*> left_wheels;
	vector <TruckPart*> right_wheels;
	TruckPart* left_steering_wheel;
	TruckPart* right_steering_wheel;
	TruckPart* body;
	Vehicle* following_vehicle;
	
	Vehicle();
	~Vehicle();

	void move(direct_t X);
	// void set_mv_matrix(glm::mat4 V);
	// void apply_mv();
};

#endif