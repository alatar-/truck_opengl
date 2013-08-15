#include "obstacle.h"
#include <vector>

using namespace std;

Obstacle::Obstacle(world_t* in_world, string in_path) : Rectangle() {
	world = in_world;
	model = new model_t();
	model->load(in_path);
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

vector <vertex_2d>  Obstacle::get_model_vertices() {
    dimensions.clear();
    vertex_2d pos_min = model->get_min_point();
    printf("vertex: ( %f ; %f ) \n", pos_min.x, pos_min.y);
    vertex_2d pos_max = model->get_max_point();
    dimensions.push_back(pos_min);
    float tmp = pos_min.y;
    pos_min.y = pos_max.y;
    printf("vertex: ( %f ; %f ) \n", pos_min.x, pos_min.y);
    dimensions.push_back(pos_min);
    printf("vertex: ( %f ; %f ) \n", pos_max.x, pos_max.y);
    dimensions.push_back(pos_max);
    pos_max.y = tmp;
    printf("vertex: ( %f ; %f ) \n", pos_max.x, pos_max.y);
    dimensions.push_back(pos_max);
    return dimensions;
}

void Obstacle::set_vertices() {
	Rectangle::set_vertices(get_model_vertices());
}