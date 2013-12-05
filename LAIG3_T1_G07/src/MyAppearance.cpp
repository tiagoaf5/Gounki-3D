#include "MyAppearance.h"
#include "CGFapplication.h"

#define CG_GLMAT_COLOR 0
#define CG_GLMAT_MATERIAL 1
#define CG_GLMAT_TEXTURE 2


MyAppearance::MyAppearance(float ambient[4], float diffuse[4], float specular[4], float emissive[4], float shininess): 
CGFappearance(ambient,diffuse,specular,shininess)
{
	this->emissive[0] = emissive[0];
	this->emissive[1] = emissive[1];
	this->emissive[2] = emissive[2];
	this->emissive[3] = emissive[3];

	texlength_s = 1;
	texlength_t = 1;
}

void MyAppearance::applyMyAppearance()
{
	CGFappearance::apply();

	if(materialType==CG_GLMAT_MATERIAL)
	{
		glMaterialfv(GL_FRONT, GL_EMISSION,  emissive);
	}
}

string MyAppearance::getId() const
{
	return id;
}

void MyAppearance::setId(string id)
{
	this->id = id;
}

void MyAppearance::setTextureId(string id)
{
	textureId = id;
}

string MyAppearance::getTextureId() const
{
	return textureId;
}

void MyAppearance::setTexLength(float s, float t)
{
	texlength_s = s;
	texlength_t = t;
}

pair <float,float> MyAppearance::getTexLength() const
{
	pair<float,float> coord;
	coord.first = texlength_s;
	coord.second = texlength_t;
	return coord;
}