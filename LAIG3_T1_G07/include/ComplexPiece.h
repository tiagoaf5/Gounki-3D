#ifndef _COMPLEXPIECE_H_
#define _COMPLEXPIECE_H_

#include <vector>
#include "Piece.h"

class ComplexPiece:public Piece{
	vector<Piece *> pieces;

public:
	ComplexPiece();
	void draw(); // vê tipos de peças e desenha conforme
};

#endif
