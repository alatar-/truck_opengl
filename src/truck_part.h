#ifndef TRUCK_PART_H
#define TRUCK_PART_H

#include <vector>
#include <string>
#include "vertex.h"

class world_t;
#include "world.h"

using namespace std;

class TruckPart {
private:
	vertex_2d pos;
	float ang_forward
		,	ang_follow
		,	ang_bend
		,	size;
	glm::mat4 in_transform,	MV;
	world_t *world;
	model_t *model;
	int last_time;
	int speed;
	glm::mat4 get_model_matrix();
public:
	TruckPart (world_t *in_world, string in_path, float in_size, float x, float y, glm::vec3 in_T, glm::vec3 in_R, glm::vec3 in_S);
	~TruckPart();
	
	void draw (bool use_mv);
	
	void set_mv_matrix (glm::mat4 V);
	void apply_mv();
	
	vertex_2d get_pos();
	void set_pos(float x, float y);
	float get_size();
	
	vector <material_t*> &get_materials() {
		return model->get_materials();
	}

	void move (float x, float y, float ang, float ds);
	void rotate (float in_ang_bend);
};

#endif