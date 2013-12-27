#ifndef _ACTION_H_
#define _ACTION_H_

#include "Piece.h"
#include "LinearAnimation.h"

class Action
{
	int x1,y1;
	int x2,y2;

	Piece * piece1;
	Piece * piece2;
	Piece * result;

	bool started;
	bool handled;
	bool finished;
	bool almostFinished;

	int nrSquares1;
	int nrSquares2;
	int nrCircles1;
	int nrCircles2;

	LinearAnimation * animation;

	void generateAnimation();
	void processEaten();
public:
	Action(int x1,int y1,int x2, int y2, Piece * p1, Piece * p2);
	void start();
	void update(unsigned long t);
	bool hasStarted() const;
	bool hasFinished() const;
	bool getAlmostFinished();
	void getCoords(int &x1,int &y1, int &x2, int &y2) const;
	Piece * getPiece(int i);
	bool getHandled() const;
	void setHandled();
	void updateBoardEaten(int &c1,int &s1, int &c2, int &s2);
};
#endif