#include "bone.h"

bone_t::bone_t() {
	parent = NULL;
	marker = false;
}

bone_t::bone_t (aiMatrix4x4 offset)
: os(offset) {
	parent = NULL;
	marker = false;
	os = offset;
}

void bone_t::add_key(trans_key_t key) {
	anim.add_key(key);
}

void bone_t::add_key(scale_key_t key) {
	anim.add_key(key);
}

void bone_t::add_key(rot_key_t key) {
	anim.add_key(key);
}

void bone_t::set_anim_full_time(float full_time) {
	anim.set_full_time(full_time);
}

void bone_t::set_parent (bone_t *new_parent) {
	parent = new_parent;
}

void bone_t::set_base_trans (aiMatrix4x4 M) {
	anim.set_base_trans(M);
}

void bone_t::set_os (aiMatrix4x4 M) {
	os = M;
}

bone_t *bone_t::get_parent() {
	return parent;
}

// void bone_t::animate(float anim_time, glm::mat4 parent_transform) {
// 	printf("\n\nbone_t::animate(%p)\n", this);
// 	printf("bone_t::animate> parent: %p\n", parent);
// 	print_mat4(parent_transform);
// 	printf("bone_t::animate> os:\n");
// 	print_mat4(os);
// 	printf("bone_t::animate> os2:\n");
// 	print_mat4(os2);
// 	accum_tr = anim.transform(anim_time);
// 	printf("bone_t::animate> anim_trans:\n");
// 	print_mat4(accum_tr);
// 	printf("bone_t::animate> accum_tr:\n");
// 	accum_tr = accum_tr * parent_transform;
// 	print_mat4(accum_tr);
// 	final_tr = os2 * accum_tr;
// 	printf("bone_t::animate> final:\n");
// 	print_mat4(final_tr);
// }

void bone_t::animate (float anim_time, aiMatrix4x4 parent_transform) {
	accum_tr = parent_transform * anim.transform(anim_time);
	final_tr = accum_tr * os;
}

aiMatrix4x4 bone_t::get_accumulated_transform() {
	return accum_tr;
}

aiMatrix4x4 bone_t::get_final_transform() {
	return final_tr;
}

bool bone_t::is_marked (bool ex_marker) {
	return ex_marker == marker;
}

void bone_t::mark (bool ex_marker) {
	marker = ex_marker;
}
