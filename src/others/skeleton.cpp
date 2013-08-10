#include "skeleton.h"

skeleton_t::skeleton_t() {
	marker = false;
}

skeleton_t::~skeleton_t() {
	clear();
}

void skeleton_t::clear() {
	for (unsigned i = 0, len = bones.size(); i < len; ++i) {
		delete bones[i];
	}
	bones.clear();
	marker = false;
}

void skeleton_t::add_bone (bone_t *new_bone) {
// 	printf("skeleton_t::add_bone(%p)\n", new_bone);
	bones.push_back(new_bone);
}

void skeleton_t::animate (float anim_time) {
// 	printf("skeleton_t::animate(%.2f): %lu bones\n", anim_time, bones.size());
	marker = !marker;
	
	for (unsigned i = 0, len = bones.size(); i < len; ++i) {
// 		printf("skeleton_t::animate> loop #%u(%p <- %p, marked: %d)\n", i, bones[i], bones[i]->get_parent(), (int)(bones[i]->is_marked(marker)));
		if (!bones[i]->is_marked(marker)) {
			animate_bone(bones[i], anim_time);
		}
	}
}

void skeleton_t::animate_bone (bone_t *bone, float anim_time) {
// 	printf("skeleton_t::animate_bone(%p <- %p,\t%.2f)\n", bone, bone->get_parent(), anim_time);
	bone->mark(marker);
	
	bone_t *parent = bone->get_parent();
	if (parent == NULL) {
		bone->animate(anim_time, global_mat);
	} else {
		if (!parent->is_marked(marker)) {
			animate_bone(parent, anim_time);
		}
		bone->animate(anim_time, parent->get_accumulated_transform());
	}
}

void skeleton_t::draw() {
// 	for (unsigned i = 0, len = bones.size(); i < len; ++i) {
// 		bones[i]->draw();
// 	}
}
