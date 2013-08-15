#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>
#include <cstdio>
#include "shared.h"

template<class T> class Vertex2D {
public:
	T x
	,	y;
	
	Vertex2D	() {
		x = 0;
		y = 0;
	}
	Vertex2D(T in) {
		x = in;
		y = in;
	}
	Vertex2D(T in_x, T in_y) {
		x = in_x;
		y = in_y;
	}

	Vertex2D<T> rotate (T angle) {
		T sn = sin(angle), cs = cos(angle);
		return Vertex2D<T>(x * cs - y * sn, x * sn + y * cs);
	}
	
	bool operator==(const Vertex2D<T> &o) const {
		return x == o.x && y == o.y;
	}
	
	bool operator!=(const Vertex2D<T> &o) const {
		return !(*this == o);
	}

	Vertex2D<T> operator+(const Vertex2D<T> &o) const {
		return Vertex2D<T>(x + o.x, y + o.y);
	}

	Vertex2D<T> operator-(const Vertex2D<T> &o) const {
		return Vertex2D<T>(x - o.x, y - o.y);
	}

	Vertex2D<T> operator+=(const Vertex2D<T> &o) {
		x += o.x;
		y += o.y;
		return *this;
	}
	
	Vertex2D<T> operator-=(const Vertex2D<T> &o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}

	Vertex2D<T> operator-() const {
		return Vertex2D<T>(-x, -y);
	}

	T operator*(const Vertex2D<T> &o){
		return x * o.x + y * o.y;
	}

	void print() {
		if (sizeof(T) == sizeof(float)) {
			printf("( %f, %f)\n", x, y);
		} else if (sizeof(T) == sizeof(double)) {
			printf("( %lf, %lf)\n", x, y);
		} else if (sizeof(T) == sizeof(long double)) {
			printf("( %Lf, %Lf)\n", x, y);
		} else {
			printf("You do what?!\n");
		}
	}

	T module(const T &o) const {
		return module(x - o.x, y - o.y);
	}
};

template<class T> class Vertex3D : public Vertex2D<T> {
public:
	T z;
	
	Vertex3D() : Vertex2D<T>() {
		
	}
	
	Vertex3D (T *arr) : Vertex2D<T>() {
		this->x = arr[0];
		this->y = arr[1];
		this->z = arr[2];
	}
	
	Vertex3D(T in_x, T in_y, T in_z) : Vertex2D<T>(in_x, in_y) {
		this->z = in_z;
	}
	
	bool operator==(const Vertex3D<T> &o) const {
		return this->x == o.x && this->y == o.y && this->z == o.z;
	}
	
	bool operator!=(const Vertex3D<T> &o) const {
		return !(*this == o);
	}
	
	Vertex3D<T> operator+(const Vertex3D<T> &o) const {
		return Vertex3D<T>(this->x + o.x, this->y + o.y, this->z + o.z);
	}
	
	Vertex3D<T> operator-(const Vertex3D<T> &o) const {
		return Vertex3D<T>(this->x - o.x, this->y - o.y, this->z - o.z);
	}
	
	Vertex3D<T> operator*(T scalar) const {
		return Vertex3D<T>(this->x * scalar, this->y * scalar, this->z * scalar);
	}
	
	T operator*(Vertex3D<T> &o) const {
		return this->x * o.x + this->y * o.y + this->z * o.z;
	}
	
	static Vertex3D<T> lerp(const Vertex3D<T> &a, const Vertex3D<T> &b, T factor) {
		return a + (b - a) * factor;
	}
};

#endif
