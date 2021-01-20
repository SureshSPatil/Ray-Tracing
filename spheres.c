#include <stdarg.h>
#include "spheres.h"

//#define NUMSPHERES 4

void initSpheres(int num, ...) {

	va_list valist;
	va_start(valist, num);

	Sphere* a = va_arg(valist, Sphere*);
	a->radius = 500;
	a->color[0] = 255;
	a->color[1] = 0;
	a->color[2] = 255;
	a->c.x = 0;
	a->c.y = 0;
	a->c.z = -500;

	Sphere* b = va_arg(valist, Sphere*);
        b->radius = 5;
        b->color[0] = 255;
        b->color[1] = 0;
        b->color[2] = 0;
        b->c.x = -9;
        b->c.y = 20;
        b->c.z = 5;

	Sphere* c = va_arg(valist, Sphere*);
        c->radius = 3;
        c->color[0] = 0;
        c->color[1] = 255;
        c->color[2] = 0;
        c->c.x = 3;
        c->c.y = 20;
        c->c.z = 3;

	Sphere* d = va_arg(valist, Sphere*);
        d->radius = 8;
        d->color[0] = 0;
        d->color[1] = 0;
        d->color[2] = 255;
        d->c.x = 0;
        d->c.y = 30;
        d->c.z = 5;
}
