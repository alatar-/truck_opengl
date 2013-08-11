#ifndef WEHICLE_H
#define WEHICLE_H

#include "truck_part.h"

class Wehicle {
public:
	TruckPart* left_steering_wheel;
	TruckPart* right_steering_wheel;
	vector <TruckPart*> left_wheels;
	vector <TruckPart*> right_wheels;
	TruckPart* body;
	
	Wehicle();
	~Wehicle();
	void set_mv_matrix (glm::mat4 V);
	void apply_mv();


};
#endif