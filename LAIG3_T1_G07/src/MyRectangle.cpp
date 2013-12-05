#include "MyRectangle.h"
#include <cmath>
#include "Data.h"

MyRectangle::MyRectangle(float x1,float y1,float x2,float y2, CGFscene * scene)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->scene = scene;
}

void MyRectangle::draw()
{
	Data * mydata = (((YAFScene*) scene)->getData());
	pair<float,float> coord = mydata->getCurrentTextureCoord();

	glBegin(GL_QUADS);
		glNormal3f(0.0,0.0,1.0);
		glTexCoord2d(0,0);
		glVertex2f(x1,y1);

		glNormal3f(0.0,0.0,1.0);
		glTexCoord2d(abs(x2-x1)/coord.first,0);
		//glTexCoord2d(1,0);
		glVertex2f(x2,y1);

		glNormal3f(0.0,0.0,1.0);
		//glTexCoord2d(1,1);
		glTexCoord2d(abs(x2-x1)/coord.first,abs(y2-y1)/coord.second);
		glVertex2f(x2,y2);

		glNormal3f(0.0,0.0,1.0);
		glTexCoord2d(0,abs(y2-y1)/coord.second);
		//glTexCoord2d(0,1);
		glVertex2f(x1,y2);
	glEnd();
}