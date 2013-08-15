#ifndef _INCLUDE_ANIM_SKELETAL_MODEL_H
#define _INCLUDE_ANIM_SKELETAL_MODEL_H

#include "model.h"
#include "anim_model.h"
#include "skeleton.h"

#include <string>
#include <map>

using namespace std;

class anim_skeletal_model_t : public anim_model_t {
protected:
	skeleton_t skeleton;
	float fps;
	
	void recalc_vertices();
	bool parse_scene (const aiScene *pScene, string &model_file);
	
	void init_bones (const aiScene *pScene);
	void map_bones (aiMesh *paiMesh, Mesh *mesh, map <string, bone_t*> &bone_map);
	void set_structure_and_animations (const aiScene *pScene, aiNode *node, bone_t *parent, map <string, bone_t*> &bone_map);
	const aiNodeAnim* find_node_anim (const aiAnimation* pAnimation, const string NodeName);
public:
	anim_skeletal_model_t();
	~anim_skeletal_model_t();
	void clear();
	
	void animate (float anim_time);
	bool load (string filename, float in_fps, unsigned in_frames);
};

#endif
