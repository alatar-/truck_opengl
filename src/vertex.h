#ifndef _INCLUDE_VERTEX_H
#define _INCLUDE_VERTEX_H

class vertex_2d {
public:
	float x
		,	y;
	
	vertex_2d() {
		
	}
	vertex_2d(float in_x, float in_y) {
		x = in_x;
		y = in_y;
	}
	
	bool operator==(const vertex_2d &o) const {
		return x == o.x && y == o.y;
	}
	
	bool operator!=(const vertex_2d &o) const {
		return !(*this == o);
	}

	vertex_2d operator-() const {
		return vertex_2d(-x, -y);
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
