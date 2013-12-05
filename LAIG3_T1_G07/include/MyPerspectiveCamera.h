#ifndef _MYPERSPECTIVECAMERA_H_
#define _MYPERSPECTIVECAMERA_H_

#include "cgfcamera.h"
#include <string>

using std::string;

class MyPerspectiveCamera : public CGFcamera
{
private:
	float angle1;
	float near;
	float far;
	string id;
public:
	MyPerspectiveCamera(float near, float far, float angle, float target[3], float pos[3]);
	void applyView();
	void updateProjectionMatrix(int width, int height);
	void setId (string name);
	string getId() const;
};

#endif