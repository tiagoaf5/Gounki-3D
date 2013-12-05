#include "MyPerspectiveCamera.h"
#include "cgfapplication.h"

using std::string;

MyPerspectiveCamera::MyPerspectiveCamera(float near, float far, float angle, float target[3], float pos[3])
	: CGFcamera()
{
	this->near = near;
	this->far = far;
	this->angle1 = angle;

	this->target [0] = target[0];
	this->target [1] = target[1];
	this->target [2] = target[2];

	this->position [0] = pos[0];
	this->position [1] = pos[1];
	this->position [2] = pos[2];
}


void MyPerspectiveCamera::applyView()
{
	gluLookAt(position[0], position[1], position[2], target[0],
		target[1], target[2],0,1,0);
/*
	glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);*/
}

void MyPerspectiveCamera::updateProjectionMatrix(int width, int height)
{
	float aspect= (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle1, aspect, near, far);
	//glMatrixMode(GL_MODELVIEW);
}

void MyPerspectiveCamera::setId (string name)
{
	id = name;
}

string MyPerspectiveCamera::getId() const
{
	return id;
}