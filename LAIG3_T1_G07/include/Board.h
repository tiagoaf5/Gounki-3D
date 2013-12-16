#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.h"


class Board
{
	MyAppearance * appearanceBlack;
	MyAppearance * appearanceWhite;

	vector<vector<Piece *>> tabuleiro;

public:
	Board();
	void generateBoard();
};

#endif
