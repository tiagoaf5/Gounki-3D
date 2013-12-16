#ifndef _COMPLEXPIECE_H_
#define _COMPLEXPIECE_H_

#include <vector>
#include "Piece.h"

class ComplexPiece:public Piece{
	vector<Piece *> pieces;

public:
	ComplexPiece();
	void draw(); // v� tipos de pe�as e desenha conforme
};

#endif
