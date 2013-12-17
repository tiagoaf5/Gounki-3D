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
	std::string getFormattedPiece(); // obtem a peça e transforma em string
	void draw();
	void addPiece(PieceBase * piece);
};


#endif