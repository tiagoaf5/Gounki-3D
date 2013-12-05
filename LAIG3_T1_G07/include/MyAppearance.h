#ifndef _MYAPPEARANCE_H_
#define _MYAPPEARANCE_H_

#include "cgfappearance.h"
#include <string>

using namespace std;


//CGFappearance lacks emissive parameter and virtual methods
class MyAppearance : public CGFappearance
{
private:
	string id;
	string textureId;
	float emissive[4];
	float texlength_s, texlength_t;
public:
	MyAppearance(float ambient[4], float diffuse[4], float specular[4], float emissive1[4], float shininess);
	void applyMyAppearance();
	string getId() const;
	void setId(string id);
	void setTextureId(string id);
	string getTextureId() const;
	void setTexLength(float s, float t);
	pair <float,float> getTexLength() const;
};



#endif