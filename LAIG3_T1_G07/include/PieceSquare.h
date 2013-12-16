#ifndef _PIECESQUARE_H_
#define _PIECESQUARE_H_

#include "PieceBase.h"
#include "Plane.h"

class PieceSquare: public PieceBase
{
private:
	Plane * side;
public:
	PieceSquare();
	std::string getFormattedPiece(); // obtem a peça e transforma em string
	void draw();

};

#endif