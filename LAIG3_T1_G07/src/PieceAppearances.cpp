#include "PiecesApperances.h"


PieceAppearances::PieceAppearances(CGFappearance * w,CGFappearance * b,CGFappearance * s) 
{
	white = w;
	black = b;
	selected = s;
}

void PieceAppearances::apply(int player)
{
	if(player == 1)
		white->apply();
	else if(player == 2)
		black->apply();
	else
		selected->apply();
}

void PieceAppearances::setAppearances(CGFappearance * w,CGFappearance * b)
{
	white = w;
	black = b;
}