#ifndef _INCLUDE_WORLD_H
#define _INCLUDE_WORLD_H

#include <vector>
#include <string>
#include <algorithm>

#include "shared.h"
#include "model.h"

class World;

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

class World {
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
	void draw_marker (Vertex2D<float>pos, glm::mat4 V);

public:
	bool win;
	World(){}
	~World();
	
	bool load(string in_config_file, unsigned in_screen_w, unsigned in_screen_h);
	void clear();
	void draw();
	void next_frame (direct_t keys_h, direct_t keys_v, direct_t height, direct_t vehicle, direct_t v_turn);
	
	void mouse_motion(float dang_h, float dang_v);

	bool test_colls_with_parking(Vertex2D<float>pos, Vertex2D<float>itd, float size, float height);
	
	void draw_in_material_order(glm::mat4 V);
	bool is_win();
	// void draw_with_shadows (glm::mat4 V);
    void tmp_animate();
};

#endif
