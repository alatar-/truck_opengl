#ifndef _INCLUDE_WORLD_H
#define _INCLUDE_WORLD_H

#include <vector>
#include <string>
#include <pthread.h>
#include <algorithm>

#include "shared.h"
#include "model.h"

class world_t;

class player_t;
#include "player.h"

class TruckPart;
#include "truck_part.h"

class Wehicle;
class Truck;
#include "wehicle.h"

using namespace std;

class world_t {
private: 
	model_t *galery;
	Truck* truck;
	Wehicle* first_trailer ;
	Wehicle* second_trailer;
	//vector<TruckPart*> truck;
	//model_t *truck;
	pthread_mutex_t mutex_position;
	player_t *player;
	vector <material_t*> materials;
	glm::mat4 P;
	float mouse_sensitivity_x
		,	mouse_sensitivity_y;
	bool invert_mouse_y;
	
	unsigned max_textures;
	bool shadows_working;
	
	GLuint shadow_map;
	unsigned shadow_map_size
		,	screen_w
		,	screen_h;
public:
	world_t(){
	}
	~world_t();
	
	bool load (string in_config_file, unsigned in_screen_w, unsigned in_screen_h);
	void draw();
	void next_frame (direct_t keys_h, direct_t keys_v, direct_t height);
	
	void lock();
	void unlock();
	
	void mouse_motion (float dang_h, float dang_v);

	bool test_colls_with_galery (vertex_2d pos, vertex_2d itd, float size, float height);
	
	void draw_in_material_order (glm::mat4 V);
	// void draw_with_shadows (glm::mat4 V);
};

#endif
