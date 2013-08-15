#include "obstacle.h"
#include <vector>

using namespace std;

Obstacle::Obstacle() : Rectangle() {

}

Obstacle::~Obstacle() {
	delete model;
	delete world;
}

float Obstacle::get_model_width() {
	return model->get_width();
}

float Obstacle::get_model_length() {
	return model->get_length();
}

vertex_2d Obstacle::get_model_min_point() {
	return model->get_min_point();
}

vertex_2d Obstacle::get_model_max_point() {
	return model->get_max_point();
}