#include "Piece.h"

Piece::Piece(int player, CGFappearance * appearance)
{
	this->player = player;
	this->appearance = appearance;
}

Piece::Piece(int player, PieceBase * piece, CGFappearance * appearance)
{
	this->player = player;
	this->appearance = appearance;
	pieces.push_back(piece);
}

void Piece::draw()
{
	appearance->apply();
	for (int i = 0; i < pieces.size(); i++)
		pieces[i]->draw();
}