#ifndef _MYSHADER_H_
#define _MYSHADER_H_

#include "CGFshader.h"
#include "CGFtexture.h"
#include "cgfapplication.h"


class MyShader: public CGFshader
{
private:
	CGFtexture * texturemap;
	CGFtexture * heightmap;
	GLint texturemapLoc;
	GLint heightmapLoc;
	GLint scaleLoc;
	GLint timeLoc;
	GLint speedLoc;
	float delta;
	bool reset;
	float startTime;
	float speed;
	float scale;
public:
	MyShader(const char *vsFile, const char *fsFile, const char * tex,  const char * height);
	virtual void bind(void);
	void update();
	void update(float time);	
	~MyShader(){};
	/*void setScale(float s);
	float normScale;*/
};


#endif