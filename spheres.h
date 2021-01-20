#include "point.h"

#define NUMSPHERES 4

#ifndef SPHERES_H
#define SPHERES_H
typedef struct Sphere {
	Point c;
	int color[3];
	double radius;
} Sphere;

void initSpheres(int, ...);
#endif
