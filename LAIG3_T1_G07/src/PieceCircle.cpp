#include "PieceCircle.h"

#define BASE 0.06
#define TOP 0.06
#define HEIGHT 0.025
#define SLICES 20
#define STACKS 5

PieceCircle::PieceCircle()
{
	piece = new Cylinder(BASE,TOP,HEIGHT,SLICES,STACKS);
}

std::string PieceCircle::getFormattedPiece()
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