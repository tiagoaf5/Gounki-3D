#ifndef _LINEARANIMATION_H_
#define _LINEARANIMATION_H_

#include "Animation.h"
#include <vector>

class LinearAnimation: public Animation
{
private:
	vector<vector<double>> points;
	vector<vector<double>> tangents;
	vector<double> distances;
	vector<double> time;
	double speed;
	double span;
	bool almostFinished;
	vector<double> getTangent(vector<double> p1, vector<double> p2, double *length);
	void init();
public:
	LinearAnimation(vector<vector<double>> points, double span);
	void handlerUpdate(unsigned long animT);
	bool getAlmostFinished() const;
};
#endif
