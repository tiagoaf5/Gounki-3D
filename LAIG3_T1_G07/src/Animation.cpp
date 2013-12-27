#include "Animation.h"


Animation::Animation()
{
	doReset=true;
	stopped = false;
	finished = false;
	//initialize transforms with identity matrix
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, &transforms[0][0]);
	glPopMatrix();
}

void Animation::start(unsigned long t)
{
	startTime = t;
	stopped = false;
	doReset = false;
}

void Animation::reset()
{
	doReset = true;
	stopped = false;
}

void Animation::stop()
{
	stopped = true;
}

bool Animation::update(unsigned long t)
{
	if(!stopped)
	{
		if (doReset)
			start(t);
		else
		{
			unsigned long animT=t-startTime;
			printf("->%lu<-\n",animT);
			handlerUpdate(animT);
		}
		return true;
	}
	return false;
}

void Animation::init()
{

}
void Animation::handlerUpdate(unsigned long animT)
{
}

void Animation::applyTransforms()
{
	glMultMatrixf(&transforms[0][0]);
}

bool Animation::isFinished() const
{
	return finished;
}