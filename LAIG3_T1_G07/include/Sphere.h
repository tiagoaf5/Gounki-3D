#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "cgfobject.h"

class Sphere: public CGFobject {

private:
	float radius;
	int slices,stacks;
	GLUquadricObj * pQuadric;
public:
	Sphere(float r1,float sl1,float st1);
	void draw();

};

#endif