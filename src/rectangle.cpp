#include "rectangle.h"
#include <cstdio>

float max(float* tab, int size) {
	float tmp = tab[0];
	for (int i = 1 ; i < size ; ++i) {
		if (tab[i] > tmp) tmp = tab[i];
	}
	return tmp;
}


float min(float* tab, int size) {
	float tmp = tab[0];
	for (int i = 1 ; i < size ; ++i) {
		if (tab[i] < tmp) tmp = tab[i];
	}
	return tmp;
}

void print_arr(float *arr, int size, const char *comment = "") {
	printf("\n%s\n", comment);
	for (int i = 0; i < size; ++i) {
		printf(" %f", arr[i]);
	}
	printf("\n");
}

Rectangle::Rectangle (vertex_2d in_ul, vertex_2d in_ur, vertex_2d in_ll, vertex_2d in_lr) {
	UL = in_ul;
	UR = in_ur;
	LL = in_ll;
	LR = in_lr;
}

Rectangle::~Rectangle() {
}

bool Rectangle::intersection(Rectangle &rect) {
	vertex_2d Axis[4];
	Axis[0].x = UR.x - UL.x;
	Axis[0].y = UR.y - UL.y;
	Axis[1].x = UR.x - LR.x;
	Axis[1].y = UR.y - LR.y;
	Axis[2].x = rect.UL.x - rect.LL.x;
	Axis[2].y = rect.UL.y - rect.LL.y;
	Axis[3].x = rect.UL.x - rect.UR.x;
	Axis[3].y = rect.UL.y - rect.UR.y;
	for(int i = 0 ; i < 4 ; ++i) { 
		float Ax_squere = Axis[i].x * Axis[i].x + Axis[i].y * Axis[i].y;
		projection[0].x = Axis[i].x * ( UR.x * Axis[i].x + UR.y * Axis[i].y ) / Ax_squere;
		projection[0].y = Axis[i].y * ( UR.x * Axis[i].x + UR.y * Axis[i].y ) / Ax_squere;
		projection[1].x = Axis[i].x * ( LR.x * Axis[i].x + LR.y * Axis[i].y ) / Ax_squere;
		projection[1].y = Axis[i].y * ( LR.x * Axis[i].x + LR.y * Axis[i].y ) / Ax_squere;
		projection[2].x = Axis[i].x * ( LL.x * Axis[i].x + LL.y * Axis[i].y ) / Ax_squere;
		projection[2].y = Axis[i].y * ( LL.x * Axis[i].x + LL.y * Axis[i].y ) / Ax_squere;
		projection[3].x = Axis[i].x * ( UL.x * Axis[i].x + UL.y * Axis[i].y ) / Ax_squere;
		projection[3].y = Axis[i].y * ( UL.x * Axis[i].x + UL.y * Axis[i].y ) / Ax_squere;
		for(int j = 0; j < 4; ++j) {
			points[j] = projection[j]*Axis[i];
		}
		rect.projection[0].x = Axis[i].x * ( rect.UR.x * Axis[i].x + rect.UR.y * Axis[i].y ) / Ax_squere;
		rect.projection[0].y = Axis[i].y * ( rect.UR.x * Axis[i].x + rect.UR.y * Axis[i].y ) / Ax_squere;
		rect.projection[1].x = Axis[i].x * ( rect.LR.x * Axis[i].x + rect.LR.y * Axis[i].y ) / Ax_squere;
		rect.projection[1].y = Axis[i].y * ( rect.LR.x * Axis[i].x + rect.LR.y * Axis[i].y ) / Ax_squere;
		rect.projection[2].x = Axis[i].x * ( rect.LL.x * Axis[i].x + rect.LL.y * Axis[i].y ) / Ax_squere;
		rect.projection[2].y = Axis[i].y * ( rect.LL.x * Axis[i].x + rect.LL.y * Axis[i].y ) / Ax_squere;
		rect.projection[3].x = Axis[i].x * ( rect.UL.x * Axis[i].x + rect.UL.y * Axis[i].y ) / Ax_squere;
		rect.projection[3].y = Axis[i].y * ( rect.UL.x * Axis[i].x + rect.UL.y * Axis[i].y ) / Ax_squere;
		for(int j = 0; j < 4; ++j) {
			rect.points[j] = rect.projection[j]*Axis[i];
		}
		if(!((min(points, 4) <= max(rect.points, 4) && min(rect.points, 4) <= max(points, 4)) || (min(rect.points, 4) <= max(points, 4) && min(points, 4) <= max(rect.points, 4)))) {
			return false;
		}
	}
	return true;
}

bool Rectangle::has_point_inside(vertex_2d &in_vertex) {
	vertex_2d Axis[2];
	Axis[0].x = UR.x - UL.x;
	Axis[0].y = UR.y - UL.y;
	Axis[1].x = UR.x - LR.x;
	Axis[1].y = UR.y - LR.y;
	vertex_2d inv_projection;
	float point;
	for(int i = 0 ; i < 2 ; ++i) { 
		float Ax_squere = Axis[i].x * Axis[i].x + Axis[i].y * Axis[i].y;
		projection[0].x = Axis[i].x * ( UR.x * Axis[i].x + UR.y * Axis[i].y ) / Ax_squere;
		projection[0].y = Axis[i].y * ( UR.x * Axis[i].x + UR.y * Axis[i].y ) / Ax_squere;
		projection[1].x = Axis[i].x * ( LR.x * Axis[i].x + LR.y * Axis[i].y ) / Ax_squere;
		projection[1].y = Axis[i].y * ( LR.x * Axis[i].x + LR.y * Axis[i].y ) / Ax_squere;
		projection[2].x = Axis[i].x * ( LL.x * Axis[i].x + LL.y * Axis[i].y ) / Ax_squere;
		projection[2].y = Axis[i].y * ( LL.x * Axis[i].x + LL.y * Axis[i].y ) / Ax_squere;
		projection[3].x = Axis[i].x * ( UL.x * Axis[i].x + UL.y * Axis[i].y ) / Ax_squere;
		projection[3].y = Axis[i].y * ( UL.x * Axis[i].x + UL.y * Axis[i].y ) / Ax_squere;
		for(int j = 0; j < 4; ++j) {
			points[j] = projection[j]*Axis[i];
		}
		inv_projection.x = Axis[i].x * ( in_vertex.x * Axis[i].x + in_vertex.y * Axis[i].y ) / Ax_squere;
		inv_projection.y = Axis[i].y * ( in_vertex.x * Axis[i].x + in_vertex.y * Axis[i].y ) / Ax_squere;
		point = inv_projection*Axis[i];

		if( !(min(points, 4) <= point && point <= max(points, 4) ) ) {
			return false;
		}
	}
	return true;


}

bool Rectangle::full_inclusion(Rectangle &rect) {
	if(	
			has_point_inside(rect.UL) 
		&&	has_point_inside(rect.UR) 
		&&	has_point_inside(rect.LL) 
		&&	has_point_inside(rect.LR) 
	) {
		return true;	
	}
	return false;
	
}