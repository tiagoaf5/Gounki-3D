#ifndef _WATERLINE_H_
#define _WATERLINE_H_

#include "cgfobject.h"
#include "MyShader.h"
#include "YAFScene.h"
#include "Plane.h"

class Waterline: public CGFobject
{
private:
	MyShader *shader;
	int parts;
	CGFobject * plane;
public:
	Waterline(const char * tex,  const char * height, const char *vsFile, const char *fsFile, CGFscene * scene);
	void draw();
	MyShader * getShader() const {return shader;};
};

#endif