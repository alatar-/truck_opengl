#ifndef _INCLUDE_ANIMATION_H
#define _INCLUDE_ANIMATION_H

#include "shared.h"
#include "vertex.h"
#include <vector>

using namespace std;

typedef struct {
	float time;
	vertex_3d<float>trans;
} trans_key_t;

typedef struct {
	float time;
	vertex_3d<float>scale;
} scale_key_t;

typedef struct {
	float time;
	glm::quat rot;
} rot_key_t;

template <class T> float key_bisection (vector<T> &arr, float elem, float full, unsigned &i, unsigned &j);

class animation_t {
private:
	vector <trans_key_t> translations;
	vector <scale_key_t> scalings;
	vector <rot_key_t> rotations;
	aiMatrix4x4 base_trans;
	float full_time;
	
	glm::quat get_rotation (float anim_time);
	vertex_3d<float>get_scaling (float anim_time);
	vertex_3d<float>get_translation (float anim_time);
	
public:
	animation_t();
	~animation_t();
	void add_key(trans_key_t key);
	void add_key(scale_key_t key);
	void add_key(rot_key_t key);
	void set_full_time (float in_full_time);
	
	void set_base_trans(aiMatrix4x4 M);
	
	aiMatrix4x4 transform (float anim_time);
};

#endif
