#ifndef _MYPERSPECTIVECAMERA_H_
#define _MYPERSPECTIVECAMERA_H_

#include "cgfcamera.h"
#include <string>

using std::string;

class MyPerspectiveCamera : public CGFcamera
{
private:
	float angle1;
	float near1;
	float far1;
	string id;
public:
	MyPerspectiveCamera(float near, float far1, float angle, float target[3], float pos[3]);
	void applyView();
	void updateProjectionMatrix(int width, int height);
	void setId (string name);
	string getId() const;
};

#endif