#ifndef _INCLUDE_SHARED_H
#define _INCLUDE_SHARED_H

#include <GL/glew.h>
#include <GL/freeglut.h>
// #include "libs/GLee/GLee.h"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "libs/glm/gtx/quaternion.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ImageMagick/Magick++.h>

#include "libs/INI.h"
#include <string>
#include <cerrno>
#include <fstream>

#include "vertex.h"

#define PI 3.14159265359

using namespace std;

typedef enum {
	BACK = -1
,	STOP = 0
,	FORW = 1
} direct_t;

typedef INI <string, string, string> ini_t;

float module (float a, float b);
float module (vertex_2d a, vertex_2d b);
string get_file (const char *filename);

void print_mat4 (glm::mat4 M);
void print_ai_mat4 (aiMatrix4x4 M);
void print_quat (glm::quat Q);
glm::mat4 quat_to_mat4 (glm::quat Q);

bool check_same_clock_dir (vertex_3d pt1, vertex_3d pt2, vertex_3d pt3, vertex_3d norm);
bool check_intersect_tri (vertex_3d pt1, vertex_3d pt2, vertex_3d pt3, vertex_3d linept, vertex_3d vect);

#endif
