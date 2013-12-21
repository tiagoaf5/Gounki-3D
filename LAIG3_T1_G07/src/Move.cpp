#include "Move.h"

Move::Move(int x1,int y1, int x2, int y2, Piece * p1, Piece * p2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->p1 = p1;
	this->p2 = p2;
}

Piece * Move::getOrigin(int &x1, int &y1) const
{
	x1 = this->x1;
	y1 = this->y1;
	return this->p1;
}

Piece * Move::getDestinantion(int &x2, int &y2) const
{
	x2 = this->x2;
	y2 = this->y2;
	return this->p2;
}