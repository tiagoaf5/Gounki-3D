#ifndef _OMNILIGHT_H_
#define _OMNILIGHT_H_

#include "MyLight.h"



class Omnilight: public MyLight{
public:
	Omnilight(unsigned int id,bool enabled,float location[4],float ambient[4],float diffuse[4],float specular[4]);
	void applyLight();
};







#endif