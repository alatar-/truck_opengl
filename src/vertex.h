#ifndef _INCLUDE_VERTEX_H
#define _INCLUDE_VERTEX_H

#include <cmath>
#include <cstdio>

class vertex_2d {
public:
	float x
		,	y;
	
	vertex_2d() {
		x = 0;
		y = 0;
	}
	vertex_2d(float in) {
		x = in;
		y = in;
	}
	vertex_2d(float in_x, float in_y) {
		x = in_x;
		y = in_y;
	}

	vertex_2d rotate (float angle) {
		float sn = sin(angle)
			,	cs = cos(angle);
		return vertex_2d(x * cs - y * sn, x * sn + y * cs);
	}
	
	bool operator==(const vertex_2d &o) const {
		return x == o.x && y == o.y;
	}
	
	bool operator!=(const vertex_2d &o) const {
		return !(*this == o);
	}

	vertex_2d operator+(const vertex_2d &o) const {
		return vertex_2d(x + o.x, y + o.y);
	}

	vertex_2d operator-(const vertex_2d &o) const {
		return vertex_2d(x - o.x, y - o.y);
	}

	vertex_2d operator+=(const vertex_2d &o) {
		x += o.x;
		y += o.y;
		return *this;
	}
	
	vertex_2d operator-=(const vertex_2d &o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}

	vertex_2d operator-() const {
		return vertex_2d(-x, -y);
	}

	float operator*(const vertex_2d &o){
		return x * o.x + y * o.y;
	}

	void print() const {
		printf("(%f %f)\n", x, y);
	}

};

class vertex_3d : public vertex_2d {
public:
	float z;
	
	vertex_3d() : vertex_2d() {
		
	}
	
	vertex_3d (float *arr) {
		x = arr[0];
		y = arr[1];
		z = arr[2];
	}
	
	vertex_3d(float in_x, float in_y, float in_z) : vertex_2d(in_x, in_y) {
		z = in_z;
	}
	
	bool operator==(const vertex_3d &o) const {
		return x == o.x && y == o.y && z == o.z;
	}
	
	bool operator!=(const vertex_3d &o) const {
		return !(*this == o);
	}
	
	vertex_3d operator+(const vertex_3d &o) const {
		return vertex_3d(x + o.x, y + o.y, z + o.z);
	}
	
	vertex_3d operator-(const vertex_3d &o) const {
		return vertex_3d(x - o.x, y - o.y, z - o.z);
	}
	
	vertex_3d operator*(float scalar) const {
		return vertex_3d(x * scalar, y * scalar, z * scalar);
	}
	
	float operator*(vertex_3d &o) const {
		return x * o.x + y * o.y + z * o.z;
	}
	
	static vertex_3d lerp(const vertex_3d &a, const vertex_3d &b, float factor) {
		return a + (b - a) * factor;
	}
};

#endif
