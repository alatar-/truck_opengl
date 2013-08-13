#include "wehicle.h"

Wehicle::Wehicle() {
	following_wehicle = NULL;
}

void Wehicle::move(direct_t X) {
    this->body->move(X);
    for (unsigned i = 0; i < this->left_wheels.size(); ++i) {
        this->left_wheels[i]->move(X);
        this->right_wheels[i]->move(X);
        this->left_wheels[i]->rotate((direct_t)(-X));
        this->right_wheels[i]->rotate(X);
    }
    if (this->following_wehicle) {
        this->following_wehicle->move(X);
    }
}

Wehicle::~Wehicle() {
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}

Truck::~Truck() {
	delete left_steering_wheel;
	delete right_steering_wheel;
}

void Truck::move(direct_t X) {
    this->left_steering_wheel->move(X);
    this->right_steering_wheel->move(X);
    this->left_steering_wheel->rotate((direct_t)(-X));
    this->right_steering_wheel->rotate(X);

    Wehicle::move(X);
}