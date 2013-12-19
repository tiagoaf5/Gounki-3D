#include "Piece.h"
#include <sstream> 

//test purpose
#include <iostream>
using namespace std;

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

	 ss << "-" << player;

	 return ss.str();
}

void Piece::addPieces(vector<PieceBase *> p)
{
	for(int i = 0; i < p.size(); i++)
		pieces.push_back(p[i]);
}

vector<PieceBase *> Piece::getPieces()
{
	return pieces;
}

int Piece::getPlayer() const
{
	return player;
}