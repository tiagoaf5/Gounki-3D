#ifndef _PIECECIRCLE_H_
#define _PIECECIRCLE_H_

#include "PieceBase.h"
#include "Cylinder.h"

class PieceCircle: public PieceBase
{
	Cylinder * piece;
public:
	PieceCircle();
	std::string getFormattedPiece(); // obtem a peça e transforma em string
	void draw();

};


#endif