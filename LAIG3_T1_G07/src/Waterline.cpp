#include "Waterline.h"
/*
GLfloat ctrlpoints[4][3] = {{ 0.0, 0.0, 15.0},
{ 0.0, 0.0, 0.0}, 
{ 3.0, 0.0, 15.0},
{ 3.0, 0.0, 0.0}};

GLfloat nrmlcompon[4][3] = {{  0.0, 1.0, 0.0},
{  0.0, 1.0, 0.0}, 
{  0.0, 1.0, 0.0},
{  0.0, 1.0, 0.0} };

GLfloat textpoints[4][2] = {{ 0.0, 0.0},
{ 0.0, 4.0}, 
{ 1.0, 0.0},
{ 1.0, 4.0} };*/
/*
GLfloat ctrlpoints[4][3] = {{ -0.5, 0.0, 0.5},
{ -0.5, 0.0, -0.5}, 
{ 0.5, 0.0, 0.5},
{ 0.5, 0.0, -0.5}};

GLfloat nrmlcompon[4][3] = {{  0.0, 1.0, 0.0},
{  0.0, 1.0, 0.0}, 
{  0.0, 1.0, 0.0},
{  0.0, 1.0, 0.0} };

GLfloat textpoints[4][2] = {{ 0.0, 0.0},
{ 0.0, 4.0}, 
{ 1.0, 0.0},
{ 1.0, 4.0} };*/



Waterline::Waterline(const char * tex, const char * height, const char *vsFile, const char *fsFile, CGFscene * scene)
{
	shader = new MyShader(vsFile,fsFile,tex,height);
	parts = 50;
	plane = new Plane(parts, parts/2, scene);
}

void Waterline::draw()
{
	shader->bind();

	plane->draw();

	/*
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &ctrlpoints[0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints[0][0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMapGrid2f(parts, 0.0,1.0, parts/2, 0.0,1.0); 

	glEvalMesh2(GL_FILL, 0,parts, 0,parts/2);
	*/
	shader->unbind();
}