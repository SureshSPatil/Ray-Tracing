#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "point.h"
#include "spheres.h"
#include "triangles.h"

typedef struct Vector{
	double xHat;
	double yHat;
	double zHat;
} Vector;

typedef struct Frame {
	Point c;
} Frame;


typedef struct Objects {
	Sphere* spheres[NUMSPHERES];
	Triangle* triangles[NUMTRIANGLES];
} Objects;


double dotProduct(Vector* a, Vector* b) {
	return a->xHat*b->xHat + a->yHat*b->yHat + a->zHat*b->zHat; 
}

void intersectSphere(Point* o, Objects* objs, Vector* object, double* tminPointer, int* objIndexPointer) {
	int i;
        double t, t1, t2;
	double x2, x1, x0;

	*tminPointer = -1;
	*objIndexPointer = -1;

        x2 = pow(object->xHat, 2) + pow(object->yHat, 2) + pow(object->zHat, 2);
        for(i = 0; i < NUMSPHERES; i++) {
                x1 = 2*((o->x - objs->spheres[i]->c.x)*object->xHat + (o->y - objs->spheres[i]->c.y)*object->yHat + (o->z - objs->spheres[i]->c.z)*object->zHat);

                x0 = -pow(objs->spheres[i]->radius, 2);
                x0 += pow(o->x, 2) + pow(o->y, 2) + pow(o->z, 2);
                x0 -= 2*(objs->spheres[i]->c.x*o->x + objs->spheres[i]->c.y*o->y + objs->spheres[i]->c.z*o->z);
                x0 += pow(objs->spheres[i]->c.x, 2) + pow(objs->spheres[i]->c.y, 2) + pow(objs->spheres[i]->c.z, 2);

                if((pow(x1,2) - 4*x2*x0) < 0) continue;
                else {
                        t1 = (-x1 + sqrt(pow(x1, 2) - 4*x2*x0))/(2*x2);
                        t2 = (-x1 - sqrt(pow(x1, 2) - 4*x2*x0))/(2*x2);
                        if(t1 < 0 && t2 < 0) t = -1;
                        else if(t1 < 0) t = t2;
                        else if(t2 < 0) t = t1;
                        else if(t1 < t2) t = t1;
                        else t = t2;


                        if ((t < *tminPointer || *tminPointer < 0) && t > 0) {
                                *tminPointer = t;
                        	*objIndexPointer = i;
				//printf("%f ", t);
                	}
        	}
	}
}

void vectorizeTriangle(Triangle* t, Vector* a, Vector* b, Vector* c) {
	a->xHat = t->p2.x - t->p1.x;
	a->yHat = t->p2.y - t->p1.y;
	a->zHat = t->p2.z - t->p1.z;

	b->xHat = t->p3.x - t->p2.x;
	b->yHat = t->p3.y - t->p2.y;
	b->zHat = t->p3.z - t->p2.z;

	c->xHat = t->p1.x - t->p3.x;
	c->yHat = t->p1.y - t->p3.y;
	c->zHat = t->p1.z - t->p3.z;
}

void crossProduct(Vector* a, Vector* b, Vector* result) {
	result->xHat = (a->yHat)*(b->zHat) - (a->zHat)*(b->yHat);
	result->yHat = -1*((a->xHat)*(b->zHat) - (a->zHat)*(b->xHat));
	result->zHat = (a->xHat)*(b->yHat) - (a->yHat)*(b->xHat);
}

void printVector(Vector* v) {
	printf("<%f, %f, %f>\n", v->xHat, v->yHat, v->zHat);
}

void intersectTriangle(Point* o, Objects* objs, Vector* object, double* tminPointer, int* objIndexPointer, Vector* normal) {
	int i, j, numVecs = 7;
	double t, area, sumArea;

	//*tminPointer = -1;
	//*objIndexPointer = -1;

	Point* p = (Point*) malloc(sizeof(Point));
	Vector** vecs = (Vector**) malloc(numVecs * sizeof(Vector*));

	for(i = 0; i < numVecs; i++) *(vecs + i) = (Vector*) malloc(sizeof(Vector));
	
	Vector* cross = (Vector *) malloc(sizeof(Vector));

	for(i = 0; i < NUMTRIANGLES; i++) {
		vectorizeTriangle(objs->triangles[i], *vecs, *(vecs + 1), *(vecs + 2));
		crossProduct(*vecs, *(vecs + 1), cross);
		area = 0.5*sqrt(dotProduct(cross, cross));
		// WHAT ABOUT PARALLEL TO VISION?
		t = cross->xHat*(objs->triangles[i]->p1.x - o->x) + cross->yHat*(objs->triangles[i]->p1.y - o->y) + cross->zHat*(objs->triangles[i]->p1.z - o->z);
		t /= dotProduct(cross, object);

		if(t > *tminPointer && (*tminPointer > 0 || t < 0)) continue;

		p->x = object->xHat*t + o->x;
	       	p->y = object->yHat*t + o->y;
		p->z = object->zHat*t + o->z;
		
		/*
		p->x = 7;
		p->y = 13;
		p->z = 15;
		*/

		(*(vecs + 3))->xHat = p->x - objs->triangles[i]->p1.x;
		(*(vecs + 3))->yHat = p->y - objs->triangles[i]->p1.y;
		(*(vecs + 3))->zHat = p->z - objs->triangles[i]->p1.z;

                (*(vecs + 4))->xHat = p->x - objs->triangles[i]->p2.x;
                (*(vecs + 4))->yHat = p->y - objs->triangles[i]->p2.y;
                (*(vecs + 4))->zHat = p->z - objs->triangles[i]->p2.z;

                (*(vecs + 5))->xHat = p->x - objs->triangles[i]->p3.x;
                (*(vecs + 5))->yHat = p->y - objs->triangles[i]->p3.y;
                (*(vecs + 5))->zHat = p->z - objs->triangles[i]->p3.z;
		
		/*	
		for(j = 0; j < numVecs; j++) printVector(*(vecs + j));
		printf("\n");
		*/

		sumArea = 0.0;
		for(j = 0; j < 3; j++) {
			crossProduct(*(vecs + j), *(vecs + j + 3), *(vecs + 6));
			sumArea += 0.5*sqrt(dotProduct(*(vecs + 6), *(vecs + 6)));
			//printf("sum Area: %f Area: %f\n", sumArea, area);
		}
		//printf("(%f, %d)", *tminPointer, *objIndexPointer);
		//printf("sum Area: %f Area: %f\n", sumArea, area);

		//exit(0);

		// EQUATING DOUBLES
		if(fabs(sumArea - area) <= 0.0005 && t > 0) {
			*tminPointer = t;
			*objIndexPointer = i + NUMSPHERES;

			if(normal) {
				normal->xHat = cross->xHat;
				normal->yHat = cross->yHat;
				normal->zHat = cross->zHat;
			}
		}	

	}

	for(i = numVecs - 1; i >= 0; i--) free(*(vecs + i));
	free(vecs);
	free(cross);

}


void freeObjects(Objects* objs) {
	void** p;
	for(p = (void**)(objs->spheres + NUMSPHERES - 1); p > (void**)(objs->spheres); p--) {
		free(*p);
	}

	free(*(objs->spheres));

	for(p = (void**)(objs->triangles + NUMTRIANGLES - 1); p > (void**)(objs->triangles); p--) {
		free(*p);
	}
	
	free(*(objs->triangles));

	free(objs);
}

int main() {
	
	Point* o = (Point*) malloc(sizeof(Point));
	o->x = 0;
	o->y = 0;
	o->z = 10;

	Point* ls = (Point*) malloc(sizeof(Point));
	ls->x = -20;
	ls->y = -10;
	ls->z = 20;

	Objects* objs = (Objects*) malloc(sizeof(Objects));

	Sphere* a = (Sphere*) malloc(sizeof(Sphere));
	Sphere* b = (Sphere*) malloc(sizeof(Sphere));
	Sphere* c = (Sphere*) malloc(sizeof(Sphere));
	Sphere* d = (Sphere*) malloc(sizeof(Sphere));

	initSpheres(NUMSPHERES, a, b, c, d);

	objs->spheres[0] = a;
	objs->spheres[1] = b;
	objs->spheres[2] = c;
	objs->spheres[3] = d;

	Triangle* e = (Triangle*) malloc(sizeof(Triangle));
	initTriangles(NUMTRIANGLES, e);

	objs->triangles[0] = e;

	Frame* f = (Frame*) malloc(sizeof(Frame));
	f->c.x = 0;
	f->c.y = .2;
	f->c.z = 10;
	
	int imgX = 720, imgY = 480;
	double frameX = .45, frameY = .3;
	int col, row, objIndex, oldObjI;
	int red, green, blue;

	Vector* object = (Vector*) malloc(sizeof(Vector));
	Vector* light = (Vector*) malloc(sizeof(Vector));
	Vector* normal = (Vector*) malloc(sizeof(Vector));
	
	double tmin, offset = 0.01;
	double iC;
	Point* p = (Point*) malloc(sizeof(Point));

	FILE* fp;
	fp = fopen("output.ppm", "w");
	fprintf(fp, "P3\n%d %d\n255\n", imgX, imgY);

	for(row = 0; row < imgY; row++) {
		for(col = 0; col < imgX; col++) {
			object->xHat = ((double)col - ((double)imgX)/2)*((frameX/2)/(((double)imgX)/2));
			object->yHat = (f->c.y) - (o->y);
			object->zHat = -1 * ((double)row - ((double)imgY)/2)*((frameY/2)/(((double)imgY)/2));

			//if(col == 0) printf("%f, %f, %f\n", object->xHat, object->yHat, object->zHat);

			intersectSphere(o, objs, object, &tmin, &objIndex);
			intersectTriangle(o, objs, object, &tmin, &objIndex, normal);


			if(objIndex == -1) fprintf(fp, "0 0 0 ");
			else{
				tmin -= offset;
				p->x = (tmin*object->xHat + o->x);
				p->y = (tmin*object->yHat + o->y);
				p->z = (tmin*object->zHat + o->z);

				light->xHat = (ls->x - p->x);
				light->yHat = (ls->y - p->y);
				light->zHat = (ls->z - p->z);

				oldObjI = objIndex;
				intersectSphere(p, objs, light, &tmin, &objIndex);
				intersectTriangle(p, objs, light, &tmin, &objIndex, NULL);
				
				if(objIndex == -1){
					if(objIndex < NUMSPHERES) {
						normal->xHat = p->x - objs->spheres[oldObjI]->c.x;
						normal->yHat = p->y - objs->spheres[oldObjI]->c.y;
						normal->zHat = p->z - objs->spheres[oldObjI]->c.z;
					}

					iC = dotProduct(light, normal)/(sqrt(dotProduct(light, light))*sqrt(dotProduct(normal, normal)));
					iC *= (iC < 0)? -1:1;

					if(oldObjI < NUMSPHERES) {
						red = (int)(iC*objs->spheres[oldObjI]->color[0] + 0.5);
						green = (int)(iC*objs->spheres[oldObjI]->color[1] + 0.5);
						blue = (int)(iC*objs->spheres[oldObjI]->color[2] + 0.5);
					} else {
						//printVector(normal);
						//printVector(light);
						red = (int)(iC*objs->triangles[oldObjI - NUMSPHERES]->color[0] + 0.5);
						green = (int)(iC*objs->triangles[oldObjI - NUMSPHERES]->color[1] + 0.5);
						blue = (int)(iC*objs->triangles[oldObjI - NUMSPHERES]->color[2] + 0.5);
					}

				       	fprintf(fp, "%d %d %d ", red, green, blue);
				} else fprintf(fp, "0 0 0 ");	
			}	
		}
		fprintf(fp, "\n");
	}

	free(o);
	free(ls);
	free(f);
	free(normal);
	freeObjects(objs);

	fclose(fp);
	return 0;
}
