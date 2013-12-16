#include "PieceSquare.h"

PieceSquare::PieceSquare()
{
	side = new Plane(2,2,NULL);
}

std::string PieceSquare::getFormattedPiece()
{
	return "x";
}

void PieceSquare::draw()
{

}