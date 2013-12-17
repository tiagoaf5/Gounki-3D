#include "Board.h"

#define BASE_X 11.14375
#define BASE_Y 2.125
#define BASE_Z 11.09375
#define HALF_LENGTH 0.1875

Board::Board(CGFappearance * black, CGFappearance * white)
{
	appearanceBlack = black;
	appearanceWhite = white;
	selectedPiece = NULL;
	generateBoard();
}

void Board::generateBoard()
{
	//generate lines with pieces
	vector<Piece *> line0 = buildRow(2,"o");
	vector<Piece *> line1 = buildRow(2,"x");
	vector<Piece *> line6 = buildRow(1,"o");
	vector<Piece *> line7 = buildRow(1,"x");
	vector<Piece *> middle;

	//empty lines
	for (int i = 0; i < 8; i++)
		middle.push_back(NULL);

	board.push_back(line0);
	board.push_back(line1);
	for (int i = 0; i < 4; i++)
		board.push_back(middle);

	board.push_back(line6);
	board.push_back(line7);
}

vector<Piece *> Board::buildRow(int player, string odd)
{
	vector<Piece *> pieces;
	CGFappearance * app;

	if(player == 1)
		app = appearanceWhite;
	else
		app = appearanceBlack;

	for(int i = 0; i < 8; i++)
	{
		PieceBase * base;

		if(i % 2 == 0 )
		{
			if (odd == "x")
				base = new PieceCircle();
			else
				base = new PieceSquare();
		}
		else
		{
			if (odd == "x")
				base = new PieceSquare();
			else
				base = new PieceCircle();
		}
		pieces.push_back(new Piece(player,base,app));
	}

	return pieces;
}

void Board::draw()
{
	for (int i = 0 ; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			glPushMatrix();
			glTranslatef(BASE_X + HALF_LENGTH * j,BASE_Y,BASE_Z + HALF_LENGTH * i);
			if(board[i][j] != NULL)
				board[i][j]->draw();
			glPopMatrix();
		}
	}
}

bool Board::selectPlace(int y, int x)
{
	if(selectedPiece != NULL && board[x][y] == NULL)
	{
		board[x][y] = selectedPiece;
		board[selectedX][selectedY] = NULL;
		selectedPiece = NULL;
		printf("Moving %d %d to %d %d\n",selectedX,selectedY,x,y);
	}
	else
	{
		selectedPiece = board[x][y];
		selectedX = x;
		selectedY = y;
		printf("Selecting %d %d\n",x,y);
	}
	return true;
}