#include "Plane.h"
#include <cmath>
#include "Data.h"

GLfloat ctrlpoints[4][3] = {{ -0.5, 0.0, 0.5},
{ -0.5, 0.0, -0.5}, 
{ 0.5, 0.0, 0.5},
{ 0.5, 0.0, -0.5}};

GLfloat nrmlcompon[4][3] = {{  0.0, 1.0, 0.0},
{  0.0, 1.0, 0.0}, 
{  0.0, 1.0, 0.0},
{  0.0, 1.0, 0.0} };


Plane::Plane(int parts, CGFscene * scene)
{
	this->partsU = parts;
	this->partsV = parts;
	this->scene = scene;
}

Plane::Plane(int partsU, int partsV, CGFscene * scene)
{
	this->partsU = partsU;
	this->partsV = partsV;
	this->scene = scene;
}

void Plane::draw()
{
	pair<float,float> coord;

	if(scene != NULL)
	{
		Data * mydata = (((YAFScene*) scene)->getData());
		coord = mydata->getCurrentTextureCoord();
	}
	else
	{
		coord.first = 1;
		coord.second = 1;
	}
	

	GLfloat textpoints[4][2] = {{ 0.0, 0.0},
	{ 0.0, 1.0 / coord.second}, 
	{ 1.0 / coord.first, 0.0},
	{ 1.0 / coord.first, 1.0 / coord.second} };

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &ctrlpoints[0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints[0][0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMapGrid2f(partsU, 0.0,1.0, partsV, 0.0,1.0); 

	glEvalMesh2(GL_FILL, 0,partsU, 0,partsV);

	/*free(ctrlpoints);
	free(nrmlcompon);*/
}