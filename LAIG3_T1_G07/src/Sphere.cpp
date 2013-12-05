#include "Sphere.h"



Sphere::Sphere(float r1,float sl1,float st1)
{
	radius = r1;
	slices = sl1;
	stacks = st1;
	pQuadric = gluNewQuadric();
	gluQuadricOrientation(pQuadric,GLU_OUTSIDE);
	gluQuadricTexture(pQuadric,true);
}


void Sphere::draw()
{
	gluSphere(pQuadric,radius,slices,stacks);	
}