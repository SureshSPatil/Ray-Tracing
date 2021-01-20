#include <stdarg.h>
#include "triangles.h"

#define NUMTRIANGLES 1

void initTriangles(int num, ...) {
	va_list valist;
	va_start(valist, num);

	Triangle* e = va_arg(valist, Triangle*);
	e->p1.x = 5;
	e->p1.y = 15;
	e->p1.z = 10;

	e->p2.x = 7;
	e->p2.y = 20;
	e->p2.z = 20;

	e->p3.x = 10;
	e->p3.y = 10;
	e->p3.z = 10;

	e->color[0] = 0;
	e->color[1] = 255;
	e->color[2] = 0;
}
