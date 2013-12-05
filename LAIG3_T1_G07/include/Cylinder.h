#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "cgfobject.h"

class Cylinder: public CGFobject {

private:
	float base,top,height;
	int slices,stacks;
	GLUquadricObj * quadObj;
	GLUquadricObj *disk_b;
	GLUquadricObj *disk_t;
public:
	Cylinder(float b1,float t1,float h1,int sl1,int st1);
	void draw();


};

#endif