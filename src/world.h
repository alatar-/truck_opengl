#ifndef _INCLUDE_WORLD_H
#define _INCLUDE_WORLD_H

#include <vector>
#include <string>
#include <algorithm>

#include "shared.h"
#include "model.h"

class world_t;

class Camera;
#include "camera.h"

class TruckPart;
#include "truck_part.h"

class Vehicle;
#include "vehicle.h"
#include "obstacle.h"

class Obstacle;
#include "obstacle.h"

using namespace std;

class world_t {
private: 
	model_t *parking;
	Vehicle* truck;
    Vehicle* first_trailer;
	Vehicle* second_trailer;
	Camera *camera;
	vector <material_t*> materials;
	vector <Obstacle*> obstacles;
	Obstacle* meta;
	glm::mat4 P;
	float mouse_sensitivity_x, mouse_sensitivity_y;
	bool invert_mouse_y;
	
	unsigned screen_w,	screen_h;

	model_t *red_marker;
	void draw_all_markers(glm::mat4 V);
	void draw_rectangle(Rectangle &rect, glm::mat4 V);
	void draw_marker (vertex_2d pos, glm::mat4 V);

public:
	bool win;
	world_t(){}
	~world_t();
	
	bool load(string in_config_file, unsigned in_screen_w, unsigned in_screen_h);
	void clear();
	void draw();
	void next_frame (direct_t keys_h, direct_t keys_v, direct_t height, direct_t vehicle, direct_t v_turn);
	
	void mouse_motion(float dang_h, float dang_v);

	bool test_colls_with_parking(vertex_2d pos, vertex_2d itd, float size, float height);
	
	void draw_in_material_order(glm::mat4 V);
	bool is_win();
	// void draw_with_shadows (glm::mat4 V);
};

#endif
