#include "rectangle.h"
#include <cstdio>

using namespace std;

template<class T> T max(T* tab, int size) {
	T tmp = tab[0];
	for (int i = 1 ; i < size ; ++i) {
		if (tab[i] > tmp) tmp = tab[i];
	}
	return tmp;
}


template<class T> T min(T* tab, int size) {
	T tmp = tab[0];
	for (int i = 1 ; i < size ; ++i) {
		if (tab[i] < tmp) tmp = tab[i];
	}
	return tmp;
}

template<class T> void print_arr(T *arr, int size, const char *comment = "") {
	printf("\n%s\n", comment);
	for (int i = 0; i < size; ++i) {
		printf(" %Lf", arr[i]);
	}
	printf("\n");
}

Rectangle::Rectangle() {

}

Rectangle::Rectangle (vertex_2d<float>in_ul, vertex_2d<float>in_ur, vertex_2d<float>in_ll, vertex_2d<float>in_lr) {
	UL = in_ul;
	UR = in_ur;
	LL = in_ll;
	LR = in_lr;
}

Rectangle::~Rectangle() {
}

bool Rectangle::intersection(Rectangle &rect) {
	vertex_2d<long double>Axis[4];
	vertex_2d<long double>projection[4]
		,	rect_projection[4];
	long double points[4]
		,	rect_points[4];
	Axis[0].x = (long double)UR.x - UL.x;
	Axis[0].y = (long double)UR.y - UL.y;
	Axis[1].x = (long double)UR.x - LR.x;
	Axis[1].y = (long double)UR.y - LR.y;
	Axis[2].x = (long double)rect.UL.x - rect.LL.x;
	Axis[2].y = (long double)rect.UL.y - rect.LL.y;
	Axis[3].x = (long double)rect.UL.x - rect.UR.x;
	Axis[3].y = (long double)rect.UL.y - rect.UR.y;
	for(int i = 0 ; i < 4 ; ++i) { 
		long double Ax_squere = Axis[i].x * Axis[i].x + Axis[i].y * Axis[i].y;
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
		rect_projection[0].x = Axis[i].x * ( rect.UR.x * Axis[i].x + rect.UR.y * Axis[i].y ) / Ax_squere;
		rect_projection[0].y = Axis[i].y * ( rect.UR.x * Axis[i].x + rect.UR.y * Axis[i].y ) / Ax_squere;
		rect_projection[1].x = Axis[i].x * ( rect.LR.x * Axis[i].x + rect.LR.y * Axis[i].y ) / Ax_squere;
		rect_projection[1].y = Axis[i].y * ( rect.LR.x * Axis[i].x + rect.LR.y * Axis[i].y ) / Ax_squere;
		rect_projection[2].x = Axis[i].x * ( rect.LL.x * Axis[i].x + rect.LL.y * Axis[i].y ) / Ax_squere;
		rect_projection[2].y = Axis[i].y * ( rect.LL.x * Axis[i].x + rect.LL.y * Axis[i].y ) / Ax_squere;
		rect_projection[3].x = Axis[i].x * ( rect.UL.x * Axis[i].x + rect.UL.y * Axis[i].y ) / Ax_squere;
		rect_projection[3].y = Axis[i].y * ( rect.UL.x * Axis[i].x + rect.UL.y * Axis[i].y ) / Ax_squere;
		for(int j = 0; j < 4; ++j) {
			rect_points[j] = rect_projection[j]*Axis[i];
		}

		// printf("\nAxis %u:\t\t", i);
		// Axis[i].print();
		// print_arr(points, 4, "points");
		// printf("min: %Lf \t max: %Lf\n", min(points, 4), max(points, 4));

		// print_arr(rect_points, 4, "rect_points");
		// printf("min: %Lf \t max: %Lf\n", min(rect_points, 4), max(rect_points, 4));

		// printf("overlapping: %d\n", (min(points, 4) <= max(rect_points, 4) && min(rect_points, 4) <= max(points, 4)) || (min(rect_points, 4) <= max(points, 4) && min(points, 4) <= max(rect_points, 4)));
		
		if(!((min(points, 4) <= max(rect_points, 4) && min(rect_points, 4) <= max(points, 4)) || (min(rect_points, 4) <= max(points, 4) && min(points, 4) <= max(rect_points, 4)))) {
			return false;
		}
	}
	return true;
}

bool Rectangle::has_point_inside(vertex_2d<float>&in_vertex) {
	vertex_2d<long double>Axis[2];
	vertex_2d<long double>projection[4];
	long double points[4];
	Axis[0].x = (long double)UR.x - UL.x;
	Axis[0].y = (long double)UR.y - UL.y;
	Axis[1].x = (long double)UR.x - LR.x;
	Axis[1].y = (long double)UR.y - LR.y;
	vertex_2d<long double>inv_projection;
	long double point;
	for(int i = 0 ; i < 2 ; ++i) { 
		long double Ax_squere = Axis[i].x * Axis[i].x + Axis[i].y * Axis[i].y;
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

void Rectangle::set_vertices(vector <vertex_2d<float> > in) {
	UL = in[0];
	UR = in[1];
	LL = in[2];
	LR = in[3];
}

vector <vertex_2d<float> > Rectangle::get_vertices() {
  vector <vertex_2d<float> > verts;
  verts.reserve(4);
  verts.push_back(UL);
  verts.push_back(UR);
  verts.push_back(LL);
  verts.push_back(LR);

  return verts;
}

void Rectangle::print() {
  printf("\n");
  vector<vertex_2d<float> > verts = get_vertices();
  for (unsigned i = 0; i < 4; ++i) {
    printf("vertex #%u: ( %f , %f ) \n", i, verts[i].x, verts[i].y);
  }
}
