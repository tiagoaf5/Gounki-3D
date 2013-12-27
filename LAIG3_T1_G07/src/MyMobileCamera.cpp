#include "MyMobileCamera.h"
#include "cgfapplication.h"


MyMobileCamera::MyMobileCamera(float near1, float far1, float angle, float target[3], float pos[3])
	: CGFcamera()
{
	this->near1 = near1;
	this->far1 = far1;
	this->angle1 = angle;

	this->target [0] = target[0];
	this->target [1] = target[1];
	this->target [2] = target[2];

	this->position [0] = pos[0];
	this->position [1] = pos[1];
	this->position [2] = pos[2];

	doReset=true;
	player = 2;
	stopped = true;
	alfa = 0;
}


void MyMobileCamera::applyView()
{
	gluLookAt(position[0], position[1], position[2], target[0],
		target[1], target[2],0,1,0);
}

void MyMobileCamera::updateProjectionMatrix(int width, int height)
{
	float aspect= (float)width / (float)height; 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle1, aspect, near1, far1);
}

void MyMobileCamera::start(unsigned long t)
{
	startTime = t;
	stopped = false;
	doReset = false;
}

void MyMobileCamera::start()
{
	stopped = false;
	doReset = true;
}

void MyMobileCamera::reset()
{
	doReset = true;
	stopped = false;
}

void MyMobileCamera::stop()
{
	stopped = true;
}

void MyMobileCamera::update(unsigned long t)
{
	if(!stopped)
	{
		if (doReset)
			start(t);
		else
		{
			unsigned long animT=t-startTime;
			if(player == 1)
				alfa = 3.14;
			else
				alfa = 0;
			alfa += (3.14/4)/1000.0*animT;

			if(player == 2 && alfa >= 3.14)
			{
				setPos(2);
				stopped = true;
			}
			else if(player == 1 && alfa >= 3.14*2)
			{
				setPos(1);
				stopped = true;
			}
			handlerUpdate();
		}
	}
}

void MyMobileCamera::setPos(int player)
{
	if(player == 1)
		alfa = 0.0;
	else
		alfa = 3.14;
	
	handlerUpdate();
}


void MyMobileCamera::handlerUpdate()
{
	position[0] = 11.8 + 1.75 * sin(alfa);
	position[2] = 13.5 - (1.75 - 1.75 * cos(alfa));
}

void MyMobileCamera::setPlayer(int p)
{
	player = p;
	stopped = false;
	doReset = true;
}

bool MyMobileCamera::isStopped() const
{
	return stopped;
}