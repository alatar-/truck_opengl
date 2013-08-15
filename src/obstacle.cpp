#include "obstacle.h"
#include <vector>

using namespace std;

Obstacle::Obstacle(World* in_world, string in_path) : Rectangle() {
	world = in_world;
	model = new Model();
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

Vertex2D<float>Obstacle::get_model_min_point() {
	return model->get_min_point();
}

Vertex2D<float>Obstacle::get_model_max_point() {
	return model->get_max_point();
}

vector <Vertex2D<float> >  Obstacle::get_model_vertices() {
    // printf("\n%p> Vehicle::get_body_vertices\n", this);

    dimensions.clear();
    Vertex2D<float>pos_min = get_model_min_point();
    // printf("vertex: ( %f , %f ) \n", pos_min.x, pos_min.y);
    Vertex2D<float>pos_max = get_model_max_point();
    dimensions.push_back(pos_min);
    float tmp = pos_min.y;
    pos_min.y = pos_max.y;
    // printf("vertex: ( %f , %f ) \n", pos_min.x, pos_min.y);
    dimensions.push_back(pos_min);
    pos_min.y = tmp;
    pos_min.x = pos_max.x;
    // printf("vertex: ( %f , %f ) \n", pos_min.x, pos_min.y);
    dimensions.push_back(pos_min);
    // printf("vertex: ( %f , %f ) \n", pos_max.x, pos_max.y);
    dimensions.push_back(pos_max);
    return dimensions;
}

void Obstacle::set_vertices() {
	Rectangle::set_vertices(get_model_vertices());
}