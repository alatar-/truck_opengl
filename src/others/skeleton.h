#ifndef _INCLUDE_SKELETON_H
#define _INCLUDE_SKELETON_H

#include "shared.h"
#include "bone.h"
#include <vector>

using namespace std;

class skeleton_t {
private:
	vector <bone_t*> bones;
	bool marker;
	void animate_bone (bone_t *bone, float anim_time);
	
	aiMatrix4x4 global_mat;
public:
	skeleton_t();
	~skeleton_t();
	
	void clear();
	void animate (float anim_time);
	void add_bone (bone_t *new_bone);
	
	void set_global_mat (aiMatrix4x4 M) {
		global_mat = M;
	}
	
	bone_t* operator[] (unsigned idx) {
		return bones[idx];
	}
	
	unsigned size() {
		return bones.size();
	}
	
	void draw();
};

#endif
