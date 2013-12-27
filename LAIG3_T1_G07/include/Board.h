#ifndef _BOARD_H_
#define _BOARD_H_

#include "Piece.h"
#include "MyAppearance.h"
#include "PieceBase.h"
#include "PieceCircle.h"
#include "PieceSquare.h"

#include "CGFappearance.h"

#include "Move.h"
#include "Action.h"
#include "MyMobileCamera.h"

#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <queue>

using namespace std;


class Board
{
	MyMobileCamera * camera;
	CGFappearance * appearanceBlack;
	CGFappearance * appearanceWhite;
	CGFappearance * appearanceSelected;
	Piece * selectedPiece;
	int selectedX;
	int selectedY;
	int nrSquares1,nrSquares2;
	int nrCircles1,nrCircles2;

	CGFappearance * whiteAppearance;
	CGFappearance * blackAppearance;

	vector<Piece *> CirclePieces1;
	vector<Piece *> SquarePieces1;
	vector<Piece *> CirclePieces2;
	vector<Piece *> SquarePieces2;

	vector<vector<Piece *>> board;

	stack<Move *> moves;
	queue<Action *> actions;
		
	void generateBoard();
	vector<Piece *> buildRow(int player, string odd);
	void saveMove(int x1,int y1, int x2, int y2);
	void addAction(int x1,int y1, int x2, int y2);
public:
	Board(CGFappearance * black, CGFappearance * white, CGFappearance * selecteda);
	void draw();
	string getFormatted() const;
	bool selectPlace(int x, int y, int player);
	bool removeSelection(int y, int x);
	string move(int y1, int x1, int y2, int x2);

	void processEaten( string eaten );

	bool pop();
	bool performAction(unsigned long t);

	//camera
	void setCamera(MyMobileCamera * camera);

	//movie
	void playMovie();
};

#endif
