#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.h"
#include "MyAppearance.h"
#include "PieceBase.h"
#include "PieceCircle.h"
#include "PieceSquare.h"

#include "CGFappearance.h"

#include <vector>

using namespace std;


class Board
{
	CGFappearance * appearanceBlack;
	CGFappearance * appearanceWhite;
	Piece * selectedPiece;
	int selectedX;
	int selectedY;
	vector<vector<Piece *>> board;

	void generateBoard();
	vector<Piece *> buildRow(int player, string odd);
public:
	Board(CGFappearance * black, CGFappearance * white);
	void draw();
	bool selectPlace(int x, int y);
	
};

#endif
