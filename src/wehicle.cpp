#include "wehicle.h"

// model = Truck/semi_truck.obj
// scale = 1/1/1
// size = 1
// posX = 0
// posY = 0
// single_wheels = 2
// sw1 = 1/1/1
// sw2 = 2/2/2
// double_wheels = 4
// dw1 = 3/3/3
// dw2 = 4/4/4
// dw3 = 5/5/5
// dw4 = 6/6/6

Wehicle::Wehicle() {

}

Wehicle::~Wehicle() {
	delete left_steering_wheel;
	delete right_steering_wheel;
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}