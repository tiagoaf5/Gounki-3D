#ifndef _PIECESQUARE_H_
#define _PIECESQUARE_H_

#include "PieceBase.h"
#include "Cylinder.h"
#include "cgfobject.h"
//#include <vector>


#include <string>

class PieceSquare: public PieceBase
{
private:
	CGFobject * piece;
public:
	PieceSquare();
	std::string getFormattedPiece(); // obtem a peça e transforma em string
	void draw();

};

#endif