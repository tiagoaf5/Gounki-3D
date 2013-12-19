#include "MyOrthogonalCamera.h"
#include "cgfapplication.h"

using std::string;

MyOrthogonalCamera::MyOrthogonalCamera(float near1, float far1, float left, float right, float top, float bottom)
	: CGFcamera()
{
	this->near1 = near1;
	this->far1 = far1;
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}

void MyOrthogonalCamera::applyView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left,right,bottom,top,near1,far1);
	//glOrtho(-5,20,-10,15,-15,50);
	glMatrixMode(GL_MODELVIEW);
}

void MyOrthogonalCamera::updateProjectionMatrix(int width, int height)
{
	//float aspect= (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left,right,bottom,top,near1,far1);
	//glOrtho(-1,20,-5,16,-15,50);
	glMatrixMode(GL_MODELVIEW);
}

void MyOrthogonalCamera::setId (string name)
{
	id = name;
}

string MyOrthogonalCamera::getId() const
{
	return id;
}