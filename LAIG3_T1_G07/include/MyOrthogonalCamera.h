#ifndef _MYORTHOGONALCAMERA_H_
#define _MYORTHOGONALCAMERA_H_

#include "cgfcamera.h"
#include <string>

using std::string;

class MyOrthogonalCamera : public CGFcamera
{
private:
	string id;
	float near1, far1, left, right, top, bottom;
public:
	MyOrthogonalCamera(float near1, float far1, float left, float right, float top, float bottom);
	void applyView();
	void updateProjectionMatrix(int width, int height);
	void setId (string name);
	string getId() const;
};


#endif
