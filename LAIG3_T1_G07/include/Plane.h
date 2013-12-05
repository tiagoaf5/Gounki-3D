#ifndef _PLANE_H_
#define _PLANE_H_

#include "cgfobject.h"
#include "YAFScene.h"

class Plane: public CGFobject
{
private:
	CGFscene * scene;
	int partsU;
	int partsV;
	float text_length_s, text_length_t;
public:
	Plane(int parts, CGFscene * scene);
	Plane(int partsU, int partsV, CGFscene * scene);
	void draw();
};

#endif