#ifndef _PIECESQUARE_H_
#define _PIECESQUARE_H_

#include "PieceBase.h"
#include "Cylinder.h"

#include <string>

class PieceSquare: public PieceBase
{
private:
	Cylinder * piece;
public:
	PieceSquare();
	std::string getFormattedPiece(); // obtem a pe�a e transforma em string
	void draw();

};

#endif