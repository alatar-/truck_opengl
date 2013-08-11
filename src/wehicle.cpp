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
	following_wehicle = NULL;
}

Wehicle::~Wehicle() {
	delete left_steering_wheel;
	delete right_steering_wheel;
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}

void Wehicle::set_mv_matrix(glm::mat4 V) {
    //for (unsigned i = 0, len = truck.shadow_map_sizee(); i < len; ++i) {
    //  truck[i]->set_mv_matrix(glm::mat4(1.0f));
    //  truck[i]->set_mv_matrix_to_meshes();
    //}
    this->body->set_mv_matrix(V);
    this->body->set_mv_matrix_to_meshes();
    //printf("this done\n");

    this->left_steering_wheel->set_mv_matrix(V);
    this->left_steering_wheel->set_mv_matrix_to_meshes();
    //printf("left done\n");
    this->right_steering_wheel->set_mv_matrix(V);
    this->right_steering_wheel->set_mv_matrix_to_meshes();
    //printf("right done\n");
    for (unsigned i = 0, len = this->left_wheels.size(); i < len; ++i) {
        this->left_wheels[i]->set_mv_matrix(V);
        this->left_wheels[i]->set_mv_matrix_to_meshes();
    }
    for (unsigned i = 0, len = this->right_wheels.size(); i < len; ++i) {
        this->right_wheels[i]->set_mv_matrix(V);
        this->right_wheels[i]->set_mv_matrix_to_meshes();
    }

}

