#include "PieceSquare.h"

using namespace std;

PieceSquare::PieceSquare()
{
	piece = new Cylinder(0.06,0.06,0.025,20,5);
}

string PieceSquare::getFormattedPiece()
{
	return "x";
}

void PieceSquare::draw()
{
	glPushMatrix();
	glRotatef(90,1,0,0);
	piece->draw();
	glPopMatrix();
}