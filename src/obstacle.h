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
	Vertex2D<float>pos;
	vector <Vertex2D<float> > dimensions;
public:
	Obstacle(World* world, string in_path);
	~Obstacle();
	vector <material_t*> &get_materials() {
		return model->get_materials();
	}
	float get_model_width();
	float get_model_length();
	Vertex2D<float>get_model_min_point();
	Vertex2D<float>get_model_max_point();

	Vertex2D<float>get_pos() {
		return pos;
	}
	vector <Vertex2D<float> > get_model_vertices();
	void set_vertices();
};

#endif