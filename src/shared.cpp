#include "shared.h"
#include <cmath>

int sign (float a) {
    if ( a < 0 ) {
        return -1;
    } else if ( a > 0 ) {
        return 1;
    } else {
        return 0;
    }
}

float normalize_angle (float ang) {
	return fmod(ang, 2 * PI);
}


float to_degrees (float radians) {
	return radians * 180 / PI;
}



template<class T> T module (T a, T b) {
	return sqrt(a * a + b * b);
}

string get_file (const char *filename) {
	ifstream in(filename, ios::in | ios::binary);
	if (in) {
		string contents;
		in.seekg(0, ios::end);
		contents.resize(in.tellg());
		in.seekg(0, ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw(errno);
}


void print_mat4 (glm::mat4 M) {
	for (unsigned i = 0; i < 4; ++i) {
		printf("[");
		for (unsigned j = 0; j < 4; ++j) {
			printf("\t%.2f", M[i][j]);
		}
		printf(" ]\n");
	}
}

void print_ai_mat4 (aiMatrix4x4 M) {
	for (unsigned i = 0; i < 4; ++i) {
		printf("[");
		for (unsigned j = 0; j < 4; ++j) {
			printf("\t%.2f", M[i][j]);
		}
		printf("]\n");
	}
}

void print_quat (glm::quat Q) {
	printf("(w, [x, y, z]) = (%.2f, [%.2f, %.2f, %.2f])\n", Q.w, Q.x, Q.y, Q.z);
}

glm::mat4 quat_to_mat4 (glm::quat Q) {
	glm::mat4 M(1.0f);
	float x_2 = 2 * Q.x * Q.x
		,	y_2 = 2 * Q.y * Q.y
		,	z_2 = 2 * Q.z * Q.z
		,	xy = 2 * Q.x * Q.y
		,	xz = 2 * Q.x * Q.z
		,	yz = 2 * Q.y * Q.z
		,	wx = 2 * Q.x * Q.w
		,	wy = 2 * Q.y * Q.w
		,	wz = 2 * Q.z * Q.w;
	M[0][0] = 1 - y_2 - z_2;
	M[0][1] = xy - wz;
	M[0][2] = xz + wy;
	
	M[1][0] = xy + wz;
	M[1][1] = 1 - x_2 - z_2;
	M[1][2] = yz - wx;
	
	M[2][0] = xz - wy;
	M[2][1] = yz + wx;
	M[2][2] = 1 - x_2 - y_2;
	
	return M;
}
