#ifndef _MYMOBILECAMERA_H_
#define _MYMOBILECAMERA_H_


#include "cgfcamera.h"

class MyMobileCamera : public CGFcamera
{
private:
	float angle1;
	float near1;
	float far1;
	int player;
	double alfa;
	unsigned long startTime;
	bool doReset;
	bool stopped;
	void start(unsigned long t);
	void handlerUpdate();
public:
	MyMobileCamera(float near1, float far1, float angle, float target[3], float pos[3]);
	void applyView();
	void updateProjectionMatrix(int width, int height);
	void update(unsigned long t);
	void reset();
	void stop();
	void setPos(int player);
	void start();
	void setPlayer(int p);
	bool isStopped() const;
};

#endif