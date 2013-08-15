#ifndef _INCLUDE_ANIM_MESH_MODEL_H
#define _INCLUDE_ANIM_MESH_MODEL_H

#include "model.h"
#include "anim_model.h"
#include "skeleton.h"

#include <string>

using namespace std;

class anim_mesh_Model : public anim_Model {
protected:
	vector < vector <vector <ExtraVertex> > > frames;
	
	void recalc_vertices();
	void prepare_vertices (float frame);
	bool load_frame (string filename, unsigned idx);
public:
	anim_mesh_Model();
	~anim_mesh_Model();
	void clear();
	bool load (string filename, float in_fps, unsigned in_frames);
	void animate (float anim_time);
};

#endif
