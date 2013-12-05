#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "cgfobject.h"
#include <vector>
#include "YAFScene.h"

using std::vector;

class Triangle: public CGFobject {

private:
	CGFscene * scene;
	float x1,y1,z1,x2,y2,z2,x3,y3,z3;
	vector<float> normal;
	float a,b,c,cosb, s3, t3;
	float text_length_s, text_length_t;
public:
	Triangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3, CGFscene * scene);
	void draw();


};

#endif