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
#include "PiecesApperances.h"

using namespace std;


class Board
{
	//Mobile camera
	MyMobileCamera * camera;
	
	//Pieces appearances
	PieceAppearances  * thePiecesApp;

	//eaten pieces
	int nrSquares1,nrSquares2;
	int nrCircles1,nrCircles2;
	Piece * circlePiece1;
	Piece * squarePiece1;
	Piece * circlePiece2;
	Piece * squarePiece2;

	//board
	vector<vector<Piece *>> board;

	//saved data mostly for undo and movie purpose
	stack<Move *> moves;
	queue<Action *> actions;
	
	//private functions
	void generateBoard();
	vector<Piece *> buildRow(int player, string odd);
	void saveMove(int x1,int y1, int x2, int y2);
	void addAction(int x1,int y1, int x2, int y2);
public:
	Board(PieceAppearances * thePiecesApp);
	void draw();
	string getFormatted() const;

	//Game's options
	bool selectPlace(int x, int y, int player);
	bool removeSelection(int y, int x);
	void move(int y1, int x1, int y2, int x2);
	bool getReady() const;

	//Undo
	bool pop();

	//To perform all board's actions
	bool performAction(unsigned long t);

	//camera
	void setCamera(MyMobileCamera * camera);

	//movie
	void playMovie();
};

#endif
