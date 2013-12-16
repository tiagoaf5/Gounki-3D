#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "cgfobject.h"
#include "YAFScene.h"

class Rectangle: public CGFobject
{
private:
	CGFscene * scene;
	float x1,y1,x2,y2;
	float text_length_s, text_length_t;
public:
	Rectangle(float x1,float y1,float x2,float y2, CGFscene * scene);
	void draw();
};

#endif