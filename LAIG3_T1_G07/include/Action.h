#ifndef _ACTION_H_
#define _ACTION_H_

#include "Piece.h"
#include "LinearAnimation.h"

class Action
{
	int x1,y1;
	int x2,y2;
	Piece * piece;
	bool started;
	bool finished;
	LinearAnimation * animation;
	void generateAnimation();
public:
	Action(int x1,int y1,int x2, int y2, Piece * piece);
	void start();
	void update(double t);
	bool isFinished() const;
};
#endif