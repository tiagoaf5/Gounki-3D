#ifndef _MOVE_H_
#define _MOVE_H_

#include "Piece.h"


class Move
{
	int x1;
	int y1;
	int x2;
	int y2;
	Piece * p1;
	Piece * p2;
public:
	Move(int x1,int y1, int x2, int y2, Piece * p1, Piece * p2);
	Piece * getOrigin(int &x1, int &y1) const;
	Piece * getDestinantion(int &x2, int &y2) const;
};




#endif