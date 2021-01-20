#include "point.h"

#define NUMTRIANGLES 1

#ifndef TRIANGLES_H
#define TRIANGLES_H
typedef struct Triangle {
	Point p1;
	Point p2;
	Point p3;
	int color[3];
} Triangle;

void initTriangles(int, ...);
#endif
