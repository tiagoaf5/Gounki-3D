#ifndef _PATCH_H_
#define _PATCH_H_

#include "cgfobject.h"
#include "YAFScene.h"

class Patch: public CGFobject
{
private:
	CGFscene * scene;
	int order;
	int partsU,partsV;
	float lengthU, lengthV;
	string compute;
	unsigned char mode;
	vector<vector<float> > controlPoints;
	int nrControlPoints;
	float text_length_s, text_length_t;
	//float** ctrlpoints;
	GLfloat ctrlpoints[16][3];
	//GLfloat textpoints[4][2];
public:
	Patch(int order,int partsU,int partsV,string compute,vector<vector<float> > controlPoints ,CGFscene * scene);
	void draw();
};

#endif