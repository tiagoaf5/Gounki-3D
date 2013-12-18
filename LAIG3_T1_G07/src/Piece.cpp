#include "Piece.h"
#include <sstream> 

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

string Piece::getFormattedPiece() 
{
	 stringstream ss;

	 ss << pieces[0]->getFormattedPiece();
	 for (int i = 1; i < pieces.size(); i++)
		 ss << "-" << pieces[i]->getFormattedPiece();

	 return ss.str();
}