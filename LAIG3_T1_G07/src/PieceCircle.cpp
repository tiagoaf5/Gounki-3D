#include "PieceCircle.h"

using namespace std;

PieceCircle::PieceCircle()
{
	piece = new Cylinder(0.06,0.06,0.025,20,5);
}

string PieceCircle::getFormattedPiece()
{
	return "o";
}

void PieceCircle::draw()
{
	glPushMatrix();
	glRotatef(90,1,0,0);
	piece->draw();
	glPopMatrix();
}