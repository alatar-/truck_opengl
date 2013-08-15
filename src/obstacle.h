#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "rectangle.h"
#include "world.h"
#include "model.h"
#include <vector>

using namespace std;

class Obstacle : public Rectangle {
private:
	model_t* model;
	World* world;
	vertex_2d<float>pos;
	vector <vertex_2d<float> > dimensions;
public:
	Obstacle(World* world, string in_path);
	~Obstacle();
	vector <material_t*> &get_materials() {
		return model->get_materials();
	}
	float get_model_width();
	float get_model_length();
	vertex_2d<float>get_model_min_point();
	vertex_2d<float>get_model_max_point();

	vertex_2d<float>get_pos() {
		return pos;
	}
	vector <vertex_2d<float> > get_model_vertices();
	void set_vertices();
};

#endif