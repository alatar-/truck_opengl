#include "animation.h"

animation_t::animation_t() {
	full_time = 0;
}

animation_t::~animation_t() {
	scalings.clear();
	translations.clear();
	rotations.clear();
}

void animation_t::add_key(trans_key_t key) {
	translations.push_back(key);
}

void animation_t::add_key(scale_key_t key) {
	scalings.push_back(key);
}

void animation_t::add_key(rot_key_t key) {
	rotations.push_back(key);
}

void animation_t::set_full_time (float in_full_time) {
	full_time = in_full_time;
}

aiMatrix4x4 animation_t::transform (float anim_time) {
	if (full_time <= 0) {
// 		aiMatrix4x4 I;
// 		return I;
		return base_trans;
	} else {
		aiMatrix4x4 T
			,	S
			,	R;
		glm::quat Q = get_rotation(anim_time);
		aiQuaternion aiQ;
		aiQ.w = Q.w;
		aiQ.x = Q.x;
		aiQ.y = Q.y;
		aiQ.z = Q.z;
		R = aiMatrix4x4(aiQ.GetMatrix());
		
		Vertex3D<float>v = get_scaling(anim_time);
		aiVector3t<float> ai_v(v.x, v.y, v.z);
		aiMatrix4x4::Scaling(ai_v, S);
		
		v = get_translation(anim_time);
		ai_v.x = v.x;
		ai_v.y = v.y;
		ai_v.z = v.z;
		aiMatrix4x4::Translation(ai_v, T);
		
		return T * R * S;
	}
}

glm::quat animation_t::get_rotation (float anim_time) {
	if (rotations.size() == 0) {
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	} else if (rotations.size() == 1) {
		return glm::normalize(rotations[0].rot);
	} else {
		unsigned i
			,	j;
		float factor = key_bisection(rotations, anim_time, full_time, i, j);
		if (factor == 0 || i == j || rotations[i].rot == rotations[j].rot) {
			return glm::normalize(rotations[i].rot);
		} else {
			glm::quat Q = glm::normalize(glm::mix(rotations[i].rot, rotations[j].rot, factor));
			return Q;
		}
	}
}

Vertex3D<float>animation_t::get_scaling (float anim_time) {
	if (rotations.size() == 0) {
		return Vertex3D<float>1.0f, 1.0f, 1.0f);
	} else if (rotations.size() == 1) {
		Vertex3D<float>sc = scalings[0].scale;
		return sc;
	} else {
		unsigned i
			,	j;
		float factor = key_bisection(scalings, anim_time, full_time, i, j);
		Vertex3D<float>sc = Vertex3D<float>:lerp(scalings[i].scale, scalings[j].scale, factor);
		return sc;
	}
}

Vertex3D<float>animation_t::get_translation (float anim_time) {
	if (translations.size() == 0) {
		return Vertex3D<float>0.0f, 0.0f, 0.0f);
	} else if (translations.size() == 1) {
		Vertex3D<float>tr = translations[0].trans;
		return tr;
	} else {
		unsigned i
			,	j;
		float factor = key_bisection(translations, anim_time, full_time, i, j);
		Vertex3D<float>tr = Vertex3D<float>:lerp(translations[i].trans, translations[j].trans, factor);
		return tr;
	}
}


void animation_t::set_base_trans (aiMatrix4x4 M) {
	base_trans = M;
}

template <class T> float key_bisection (vector<T> &arr, float elem, float full, unsigned &i, unsigned &j) {
	int left = 0
		,	right = arr.size() - 1
		,	mid;
	T mid_elem;
	
	while (left <= right) {
		mid = (left + right) / 2;
		mid_elem = arr[mid];
		if (*((float*)&mid_elem) >= elem) {
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}
	
	if (left == 0) {
		// obsługa czasu zerowego/ujemnego, czy interpolacji z ostatnim kluczem
		i = 0;
		j = 0;
		return 0;
	} else if (left == (int)arr.size()) {
		// ewentualnie interpolacja z zerem...
		i = arr.size() - 1;
		j = i;
		return 0;
	}
	
	j = left;
	i = j - 1;
	float ith = *((float*)&arr.at(i))
		,	jth = *((float*)&arr.at(j));
	return (elem - ith) / (jth - ith);
}
