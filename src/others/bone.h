#ifndef _INCLUDE_BONE_H
#define _INCLUDE_BONE_H

#include "shared.h"
#include "animation.h"

class bone_t {
private:
	aiMatrix4x4 os
		,	accum_tr
		,	final_tr;
	animation_t anim;
	bone_t *parent;
	bool marker;
public:
	bone_t();
	bone_t (aiMatrix4x4 offset);
	~bone_t() {}
	
	void add_key(trans_key_t key);
	void add_key(scale_key_t key);
	void add_key(rot_key_t key);
	void set_anim_full_time(float full_time);
	
	bool is_marked (bool ex_marker);
	void mark (bool ex_marker);
	
	void set_base_trans (aiMatrix4x4 M);
	void set_os (aiMatrix4x4 M);
	aiMatrix4x4 get_os() {
		return os;
	}
	
	void set_parent (bone_t *new_parent);
	bone_t *get_parent();
	void animate (float anim_time, aiMatrix4x4 parent_transform);
	aiMatrix4x4 get_accumulated_transform();
	aiMatrix4x4 get_final_transform();
};

#endif
