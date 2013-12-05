#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "cgfobject.h"
#include "YAFScene.h"

class Vehicle: public CGFobject
{
private:
	CGFscene * scene;
	CGFobject * body;
	CGFobject * head;
	CGFobject * eye;
	CGFobject * wing;
	CGFtexture * body_tex;
	CGFtexture * head_tex;
	CGFtexture * eye_tex;
	CGFtexture * wing_tex;
public:
	Vehicle(CGFscene * scene);
	void draw();
};

#endif