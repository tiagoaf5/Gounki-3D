#ifndef _PIECE_H_
#define _PIECE_H_

#include "PieceBase.h"
#include "CGFappearance.h"

#include <string>
#include <vector>

using namespace std;

class Piece
{
	CGFappearance * appearance;
	int player;
	bool selected;
	vector<PieceBase *> pieces;
public:
	Piece(int player, CGFappearance * appearance);
	Piece(int player, PieceBase * piece, CGFappearance * appearance);
	string getFormattedPiece(); // piece ready for prolog
	void draw();
	void addPieces(vector<PieceBase *> p);
	vector<PieceBase *> getPieces();
	int getPlayer() const;
};


#endif