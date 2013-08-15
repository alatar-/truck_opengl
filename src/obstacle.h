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
	world_t* world;
	vertex_2d pos;
	vector <vertex_2d> dimensions;
public:
	Obstacle(world_t* world, string in_path);
	~Obstacle();
	vector <material_t*> &get_materials() {
		return model->get_materials();
	}
	float get_model_width();
	float get_model_length();
	vertex_2d get_model_min_point();
	vertex_2d get_model_max_point();

	vertex_2d get_pos() {
		return pos;
	}
	vector <vertex_2d> get_model_vertices();
	void set_vertices();
};

#endif