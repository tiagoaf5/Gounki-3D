#include "Cylinder.h"


Cylinder::Cylinder(float b1,float t1,float h1,int sl1,int st1)
{
	base = b1;
	top = t1;
	height = h1;
	slices = sl1;
	stacks = st1;
	quadObj = gluNewQuadric();
	disk_b = gluNewQuadric();
	disk_t = gluNewQuadric();

	gluQuadricOrientation(quadObj,GLU_OUTSIDE);
	gluQuadricOrientation(disk_b,GLU_OUTSIDE);
	gluQuadricOrientation(disk_t,GLU_OUTSIDE);

	gluQuadricTexture(quadObj,true);
	gluQuadricTexture(disk_b,true);
	gluQuadricTexture(disk_t,true);
}

void Cylinder::draw()
{
	gluCylinder(quadObj, base, top, height, slices, stacks);
	
	glPushMatrix();
		glRotatef(180,0,1,0);
		gluDisk(disk_b,0,base,slices,1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,0, height);
		gluDisk(disk_t,0,top,slices,1);
	glPopMatrix();
}

