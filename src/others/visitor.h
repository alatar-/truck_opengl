#ifndef _INCLUDE_VISITOR_H
#define _INCLUDE_VISITOR_H

#include <vector>
#include <string>
#include <pthread.h>
#include "vertex.h"
#include "path.h"
#include "anim_model.h"
#include "anim_mesh_model.h"
#include "anim_skeletal_model.h"

class world_t;
#include "world.h"

using namespace std;

class visitor_t {
private:
	path_t path;
	vertex_2d pos;
	float ang_h
		,	speed
		,	wait_time
		,	size
		,	anim_time;
		
	glm::mat4 in_transform
		,	MV;
	
	bool alive
		,	animated;
	
	world_t *world;
	
	anim_model_t *model;
	
	glm::mat4 get_model_matrix();
public:
	pthread_t thread;
	
	visitor_t (world_t *in_world, string in_path, string visiting_path, bool looped_path, float in_speed, float in_wait_time, float in_size, bool in_animated, float fps, unsigned frames, bool skeletal, glm::vec3 in_T, glm::vec3 in_R, glm::vec3 in_S);
	~visitor_t();
	
	int fire();
	int join();
	void kill();
	bool move (vertex_2d itd);
	void life();
	
	void animate();
	void draw (bool use_mv);
	
	void set_mv_matrix (glm::mat4 V);
	void apply_mv();
	void set_mv_matrix_to_meshes();
	
	vertex_2d get_pos();
	float get_size();
	
	vector <material_t*> &get_materials() {
		return model->get_materials();
	}
};

void* visitor_routine(void *arg);

#endif