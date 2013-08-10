#include "anim_skeletal_model.h"

anim_skeletal_model_t::anim_skeletal_model_t() : anim_model_t() {
}

anim_skeletal_model_t::~anim_skeletal_model_t() {
	clear();
}

void anim_skeletal_model_t::clear() {
	model_t::clear();
	skeleton.clear();
}

bool anim_skeletal_model_t::load (string filename, float in_fps, unsigned in_frames) {
	fps = in_fps;
	return model_t::load(filename);
}

bool anim_skeletal_model_t::parse_scene (const aiScene *pScene, string &model_file) {
	if (model_t::parse_scene(pScene, model_file)) {
		if (pScene->mNumAnimations) {
			init_bones(pScene);
		}
		return true;
	} else {
		return false;
	}
}


void anim_skeletal_model_t::init_bones (const aiScene *pScene) {
// 	printf("anim_skeletal_model_t::init_bones\n");
	map <string, bone_t*> bone_map;
	
	{
		aiMatrix4x4 global_trans = pScene->mRootNode->mTransformation;
		global_trans.Inverse();
		global_trans.Transpose();
		skeleton.set_global_mat(global_trans);
	}
	
	for (unsigned i = 0, len = pScene->mNumMeshes; i < len; ++i) {
// 		printf("anim_skeletal_model_t::init_bones> #%u: map_bones(%p, %p)\n", i, pScene->mMeshes[i], meshes[i]);
		map_bones(pScene->mMeshes[i], meshes[i], bone_map);
	}
// 	printf("anim_skeletal_model_t::init_bones> bones_mapped, size=%lu\n", bone_map.size());
	
	if (fps == 0) {
		fps = pScene->mAnimations[0]->mTicksPerSecond;
		if (fps == 0) {
			fps = 24.0f;
		}
	}
	
	full_time = pScene->mAnimations[0]->mDuration / fps;
	set_structure_and_animations(pScene, pScene->mRootNode, NULL, bone_map);
// 	printf("anim_skeletal_model_t::init_bones> structure set\n");
}

void anim_skeletal_model_t::map_bones (aiMesh *paiMesh, mesh_t *mesh, map <string, bone_t*> &bone_map) {
// 	printf("anim_skeletal_model_t::map_bones(%p, %p)\n", paiMesh, mesh);
	for (unsigned i = 0, ilen = paiMesh->mNumBones; i < ilen; ++i) {
// 	printf("anim_skeletal_model_t::map_bones(%p, %p)> in loop #%u\n", paiMesh, mesh, i);
		const aiBone *paiBone = paiMesh->mBones[i];
// 		printf("anim_skeletal_model_t::map_bones(%p, %p)> in loop #%u, bone: %p\n", paiMesh, mesh, i, paiBone);
		string bone_name(paiBone->mName.data);
		bone_t *the_bone;
// 		printf("anim_skeletal_model_t::map_bones> pre_bad_printf\n");
// 		printf("anim_skeletal_model_t::map_bones> #%u < %u, name: %s\n", i, ilen, bone_name.c_str());
// 		printf("anim_skeletal_model_t::map_bones> post_bad_printf\n");
		
		if (bone_map.find(bone_name) == bone_map.end()) {
			the_bone = new bone_t(paiBone->mOffsetMatrix);
			skeleton.add_bone(the_bone);
			bone_map[bone_name] = the_bone;
		} else {
			the_bone = bone_map[bone_name];
		}
		
		for (unsigned j = 0, jlen = paiBone->mNumWeights; j < jlen; ++j) {
// 			printf("anim_skeletal_model_t::map_bones> weight #%u < %u\n", j, jlen);
			aiVertexWeight v_weight = paiBone->mWeights[j];
			if (v_weight.mWeight > 0.0f) {
				mesh->attach_bone(v_weight.mVertexId, the_bone, v_weight.mWeight);
			}
		}
	}
}

void anim_skeletal_model_t::set_structure_and_animations (const aiScene *pScene, aiNode *node, bone_t *parent, map <string, bone_t*> &bone_map) {
	string node_name(node->mName.data);
	const aiNodeAnim *pNodeAnim = find_node_anim(pScene->mAnimations[0], node_name);
	bone_t *the_bone;
	
// 	printf("anim_skeletal_model_t::set_structure_and_animations(%p, %p), name: %s\n", node, parent, node_name.c_str());
	
	if (bone_map.find(node_name) == bone_map.end()) {
		the_bone = new bone_t();
		skeleton.add_bone(the_bone);
// 		printf("anim_skeletal_model_t::set_structure_and_animations(%p, %p): NEW BONE: %p, name: %s\n", node, parent, the_bone, node_name.c_str());
	} else {
		the_bone = bone_map[node_name];
	}
	the_bone->set_parent(parent);
// 	printf("anim_skeletal_model_t::set_structure_and_animations> parent(%p) set to %p\n", parent, the_bone->get_parent());
	
	if (pNodeAnim) {
		the_bone->set_anim_full_time(full_time);
		
// 		printf("anim_skeletal_model_t::set_animations> pos keys: %u\n", pNodeAnim->mNumPositionKeys);
// 		printf("anim_skeletal_model_t::set_animations> scl keys: %u\n", pNodeAnim->mNumScalingKeys);
// 		printf("anim_skeletal_model_t::set_animations> rot keys: %u\n", pNodeAnim->mNumRotationKeys);
	
		for (unsigned i = 0, len = pNodeAnim->mNumPositionKeys; i < len; ++i) {
			const aiVectorKey *pKey = &pNodeAnim->mPositionKeys[i];
			trans_key_t key;
			key.time = pKey->mTime / fps;
			key.trans.x = pKey->mValue.x;
			key.trans.y = pKey->mValue.y;
			key.trans.z = pKey->mValue.z;
			the_bone->add_key(key);
		}
		
		for (unsigned i = 0, len = pNodeAnim->mNumScalingKeys; i < len; ++i) {
			const aiVectorKey *pKey = &pNodeAnim->mScalingKeys[i];
			scale_key_t key;
			key.time = pKey->mTime / fps;
			key.scale.x = pKey->mValue.x;
			key.scale.y = pKey->mValue.y;
			key.scale.z = pKey->mValue.z;
			the_bone->add_key(key);
		}
		
		for (unsigned i = 0, len = pNodeAnim->mNumRotationKeys; i < len; ++i) {
			const aiQuatKey *pKey = &pNodeAnim->mRotationKeys[i];
			rot_key_t key;
			key.time = pKey->mTime / fps;
// 			key.rot.w = pKey->mValue.w;
// 			key.rot.x = pKey->mValue.x;
// 			key.rot.y = pKey->mValue.y;
// 			key.rot.z = pKey->mValue.z;
			key.rot.w = pKey->mValue.w;
			key.rot.x = pKey->mValue.x;
			key.rot.y = pKey->mValue.y;
			key.rot.z = pKey->mValue.z;
			the_bone->add_key(key);
		}
	} else {
		the_bone->set_anim_full_time(0);
// 		printf("anim_skeletal_model_t::set_structure_and_animations(%p, %p): NO ANIMATION on bone(%p), name: %s\n", node, parent, the_bone, node_name.c_str());
	}
	
// 	{
// 		aiMatrix4x4 OS = node->mTransformation;
// 		OS.Inverse();
// 		if (the_bone->get_parent() == NULL) {
// 			the_bone->set_os(OS);
// 		} else {
// 			the_bone->set_os(OS * the_bone->get_parent()->get_os());
// 		}
// 	}
	if (the_bone->get_parent()) {
		the_bone->set_base_trans(node->mTransformation);
	}
	
// 		printf("Node transform\n");
		print_ai_mat4(node->mTransformation);
	
// 	printf("anim_skeletal_model_t::set_structure_and_animations> children: %u\n", node->mNumChildren);
	for (unsigned i = 0, len = node->mNumChildren; i < len; ++i) {
		set_structure_and_animations(pScene, node->mChildren[i], the_bone, bone_map);
	}
}

const aiNodeAnim* anim_skeletal_model_t::find_node_anim (const aiAnimation* pAnimation, const string NodeName) {
// 	printf("anim_skeletal_model_t::find_node_anim(%s)\n", NodeName.c_str());
	for (unsigned i = 0, len = pAnimation->mNumChannels; i < len; ++i) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
			
			if (string(pNodeAnim->mNodeName.data) == NodeName) {
					return pNodeAnim;
			}
	}
	
	return NULL;
}

void anim_skeletal_model_t::recalc_vertices() {
// 	printf("anim_skeletal_model_t::recalc_vertices\n");
	for (unsigned i = 0, len = meshes.size(); i < len; ++i) {
		meshes[i]->recalc();
		meshes[i]->set_vertices_data();
	}
}

void anim_skeletal_model_t::animate (float anim_time) {
	if (full_time) {
// 		printf("anim_mesh_model_t::animate(%.2f)> %.2f\n", anim_time, fmod(anim_time, full_time));
		skeleton.animate(fmod(anim_time, full_time));
		recalc_vertices();
	}
}
