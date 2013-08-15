#ifndef _INCLUDE_ANIM_MODEL_H
#define _INCLUDE_ANIM_MODEL_H

#include <string>
#include "model.h"

using namespace std;

class anim_Model : public Model {
protected:
	float full_time;
public:
	anim_Model();
	
	virtual void animate (float anim_time) = 0;
	virtual bool load (string filename, float in_fps, unsigned in_frames) = 0;
};

#endif
