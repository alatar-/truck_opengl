#include "shared.h"
#include <cmath>

float module (float a, float b) {
	return sqrt(a * a + b * b);
}

float module (vertex_2d a, vertex_2d b) {
	return module(a.x - b.x, a.y - b.y);
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

bool check_same_clock_dir (vertex_3d pt1, vertex_3d pt2, vertex_3d pt3, vertex_3d norm) {
	float testi, testj, testk;
	float dotprod;
	// normal of trinagle
	testi = (((pt2.y - pt1.y)*(pt3.z - pt1.z)) - ((pt3.y - pt1.y)*(pt2.z - pt1.z)));
	testj = (((pt2.z - pt1.z)*(pt3.x - pt1.x)) - ((pt3.z - pt1.z)*(pt2.x - pt1.x)));
	testk = (((pt2.x - pt1.x)*(pt3.y - pt1.y)) - ((pt3.x - pt1.x)*(pt2.y - pt1.y)));

	// Dot product with triangle normal
	dotprod = testi * norm.x + testj * norm.y + testk * norm.z;

	//answer
	return dotprod >= 0;
}

bool check_intersect_tri (vertex_3d pt1, vertex_3d pt2, vertex_3d pt3, vertex_3d linept, vertex_3d vect) {
	vertex_3d V1
		,	V2
		,	norm;
	float dotprod;
	float t;

	// vector form triangle pt1 to pt2
	V1 = pt2 - pt1;

	// vector form triangle pt2 to pt3
	V2 = pt3 - pt1;

	// vector normal of triangle
	norm.x = V1.y * V2.z - V1.z * V2.y;
	norm.y = V1.z * V2.x - V1.x * V2.z;
	norm.z = V1.x * V2.y - V1.y * V2.x;

	// dot product of normal and line's vector if zero line is parallel to triangle
	dotprod = norm.x * vect.x + norm.y * vect.y + norm.z * vect.z;

	if(dotprod < 0) {
		//Find point of intersect to triangle plane.
		//find t to intersect point
		t = (norm.x*vect.x+norm.y*vect.y+norm.z*vect.z);
		if (t == 0) {
			return false;
		}
		t = -(norm.x*(linept.x-pt1.x)+norm.y*(linept.y-pt1.y)+norm.z*(linept.z-pt1.z)) / t;

		// if ds is neg line started past triangle so can't hit triangle.
		if(t < 0 || t > 1) return false;

		V1 = linept + vect * t;

		return check_same_clock_dir(pt1, pt2, V1, norm)
			&& check_same_clock_dir(pt2, pt3, V1, norm)
			&& check_same_clock_dir(pt3, pt1, V1, norm);
	}
	return false;
}
