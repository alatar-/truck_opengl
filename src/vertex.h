#ifndef _INCLUDE_VERTEX_H
#define _INCLUDE_VERTEX_H

#include <cmath>
#include <cstdio>

template<class T> class vertex_2d {
public:
	T x
	,	y;
	
	vertex_2d	() {
		x = 0;
		y = 0;
	}
	vertex_2d(T in) {
		x = in;
		y = in;
	}
	vertex_2d(T in_x, T in_y) {
		x = in_x;
		y = in_y;
	}

	vertex_2d<T> rotate (T angle) {
		T sn = sin(angle)
			,	cs = cos(angle);
		return vertex_2d<T>(x * cs - y * sn, x * sn + y * cs);
	}
	
	bool operator==(const vertex_2d<T> &o) const {
		return x == o.x && y == o.y;
	}
	
	bool operator!=(const vertex_2d<T> &o) const {
		return !(*this == o);
	}

	vertex_2d<T> operator+(const vertex_2d<T> &o) const {
		return vertex_2d<T>(x + o.x, y + o.y);
	}

	vertex_2d<T> operator-(const vertex_2d<T> &o) const {
		return vertex_2d<T>(x - o.x, y - o.y);
	}

	vertex_2d<T> operator+=(const vertex_2d<T> &o) {
		x += o.x;
		y += o.y;
		return *this;
	}
	
	vertex_2d<T> operator-=(const vertex_2d<T> &o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}

	vertex_2d<T> operator-() const {
		return vertex_2d<T>(-x, -y);
	}

	T operator*(const vertex_2d<T> &o){
		return x * o.x + y * o.y;
	}

	void print() const {
		printf("(%f %f)\n", x, y);
	}

};

template<class T> class vertex_3d : public vertex_2d<T> {
public:
	T z;
	
	vertex_3d() : vertex_2d<T>() {
		
	}
	
	vertex_3d (T *arr) : vertex_2d<T>() {
		this->x = arr[0];
		this->y = arr[1];
		this->z = arr[2];
	}
	
	vertex_3d(T in_x, T in_y, T in_z) : vertex_2d<T>(in_x, in_y) {
		this->z = in_z;
	}
	
	bool operator==(const vertex_3d<T> &o) const {
		return this->x == o.x && this->y == o.y && this->z == o.z;
	}
	
	bool operator!=(const vertex_3d<T> &o) const {
		return !(*this == o);
	}
	
	vertex_3d<T> operator+(const vertex_3d<T> &o) const {
		return vertex_3d<T>(this->x + o.x, this->y + o.y, this->z + o.z);
	}
	
	vertex_3d<T> operator-(const vertex_3d<T> &o) const {
		return vertex_3d<T>(this->x - o.x, this->y - o.y, this->z - o.z);
	}
	
	vertex_3d<T> operator*(T scalar) const {
		return vertex_3d<T>(this->x * scalar, this->y * scalar, this->z * scalar);
	}
	
	T operator*(vertex_3d<T> &o) const {
		return this->x * o.x + this->y * o.y + this->z * o.z;
	}
	
	static vertex_3d<T> lerp(const vertex_3d<T> &a, const vertex_3d<T> &b, T factor) {
		return a + (b - a) * factor;
	}
};

#endif
