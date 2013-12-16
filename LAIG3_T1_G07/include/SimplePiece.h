#ifndef _SIMPLEPIECE_H_
#define _SIMPLEPIECE_H_

#include <vector>
#include "Piece.h"

class SimplePiece:public Piece{
	int type; // 0 is a circle, 1 is a square
public:
	SimplePiece();
	void draw(); // desenha
};

#endif
