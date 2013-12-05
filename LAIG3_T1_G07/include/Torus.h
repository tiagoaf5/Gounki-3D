#ifndef _TORUS_H_
#define _TORUS_H_

#include "cgfobject.h"

class Torus: public CGFobject {

private:
	float inner,outer;
	int slices,loops;
	// A texture
	float TextureWrapVert;
	float TextureWrapHoriz;
	float PI2;
	void Torus::putVertTexture(int i, int j);
public:
	Torus(float i1,float o1,int sl1,int l1);
	void draw();


};

#endif