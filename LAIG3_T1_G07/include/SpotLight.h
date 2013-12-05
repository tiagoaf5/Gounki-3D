#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "MyLight.h"



class SpotLight: public MyLight{
public:
	SpotLight(unsigned int id,bool enabled,float location[4],float ambient[4],float diffuse[4],float specular[4],float angle,float exponent,float direction[3]);
	void applyLight();
private:
	int exponent;
};



#endif