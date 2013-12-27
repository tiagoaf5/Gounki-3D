#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "CGFApplication.h"

class Animation
{
private:
	unsigned long startTime;
	bool doReset;
	bool stopped;
	void start(unsigned long t);
	virtual void init();
protected:
	float transforms[4][4];
	bool finished;
public:
	Animation();
	bool update(unsigned long t);
	void reset();
	void stop();
	virtual void handlerUpdate(unsigned long animT);
	void applyTransforms();
	bool isFinished() const;
};



#endif