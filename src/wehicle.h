#ifndef WEHICLE_H
#define WEHICLE_H

#include "truck_part.h"

class Wehicle {
public:
	vector <TruckPart*> left_wheels;
	vector <TruckPart*> right_wheels;
	TruckPart* body;
	Wehicle* following_wehicle;
	
	Wehicle();
	~Wehicle();
	void set_mv_matrix (glm::mat4 V);
	void apply_mv();


};

class Truck : public Wehicle {
public:
	TruckPart* left_steering_wheel;
	TruckPart* right_steering_wheel;
	Truck() : Wehicle() {
		printf("Truck made\n");
	}
	~Truck();
};
#endif