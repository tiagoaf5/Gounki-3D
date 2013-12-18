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
	 cout << "val-> " <<pieces[0]->getFormattedPiece() << endl;
	 for (int i = 1; i < pieces.size(); i++)
		 ss << "-" << pieces[i]->getFormattedPiece();

	 ss << "-" << player;

	 cout << "result-> " << ss.str() << endl;
	 return ss.str();
}