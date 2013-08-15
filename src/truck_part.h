#ifndef TRUCK_PART_H
#define TRUCK_PART_H

#include <vector>
#include <string>
#include "vertex.h"

class World;
#include "world.h"

using namespace std;

class TruckPart {
private:
	Vertex2D<float>pos;
	float ang_forward
		,	ang_follow
		,	ang_bend
		,	size;
	glm::mat4 in_transform,	MV;
	World *world;
	Model *model;
	int last_time;
	int speed;
	glm::mat4 get_model_matrix();
public:
	TruckPart (World *in_world, string in_path, float in_size, float x, float y, glm::vec3 in_T, glm::vec3 in_R, glm::vec3 in_S);
	~TruckPart();
	
	void draw (bool use_mv);
	
	void set_mv_matrix (glm::mat4 V);
	void apply_mv();
	
	Vertex2D<float>get_pos();
	void set_pos(float x, float y);
	float get_size();
	
	vector <Material*> &get_materials() {
		return model->get_materials();
	}

	void move (Vertex2D<float>position, float ang, float ds);
	void rotate (float in_ang_bend);
	float get_model_width();
	float get_model_length();
	Vertex2D<float>get_model_min_point();
	Vertex2D<float>get_model_max_point();
};

#endif