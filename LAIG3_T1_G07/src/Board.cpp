#include "Board.h"

#define BASE_X 11.14375
#define BASE_Y 2.125
#define BASE_Z 11.09375
#define HALF_LENGTH 0.1875
#define SIZE 8
#define BASE_X2 10.9
#define BASE_Y2 2.02
#define BASE_Z2 11.4
#define HALF_LENGTH2 0.03

//test purpose
#include <iostream>
using namespace std;

Board::Board(PieceAppearances * thePiecesApp)
{
	/*blackAppearance = black;
	whiteAppearance = white;

	appearanceBlack = black;
	appearanceWhite = white;
	appearanceSelected = selecteda;*/
	//selectedPiece = NULL;
	this->thePiecesApp = thePiecesApp;
	generateBoard();

	PieceCircle *pc1;
	PieceSquare *ps1;
	PieceCircle *pc2;
	PieceSquare *ps2;

	nrSquares1 = 0;
	nrSquares2 = 0;
	nrCircles1 = 0;
	nrCircles2 = 0;

	PieceBase * pc = new PieceCircle();
	circlePiece1 = new Piece(1,pc,thePiecesApp);
	circlePiece2 = new Piece(2,pc,thePiecesApp);
	pc = new PieceSquare();
	squarePiece1 = new Piece(1,pc,thePiecesApp);
	squarePiece2 = new Piece(2,pc,thePiecesApp);

	/*
	CGFappearance * app;

	for (int i = 0 ; i < 8 ; i++){
	pc1 = new PieceCircle();

	app = blackAppearance;

	Piece* pie = new Piece(1,pc1,app,selecteda);

	pie->setHidden(0);

	CirclePieces1.push_back(pie);
	}


	//square p1
	for (int i = 0 ; i < 8 ; i++){
	ps1 = new PieceSquare();

	app = blackAppearance;

	Piece* pie1 = new Piece(1,ps1,app,selecteda);

	pie1->setHidden(0);

	SquarePieces1.push_back(pie1);
	}

	//circle p2
	for (int i = 0 ; i < 8 ; i++){
	pc2 = new PieceCircle();

	app = whiteAppearance;

	Piece* pie2 = new Piece(1,pc2,app,selecteda);

	pie2->setHidden(0);

	CirclePieces2.push_back(pie2);
	}


	//square p2
	for (int i = 0 ; i < 8 ; i++){
	ps2 = new PieceSquare();

	app = whiteAppearance;

	Piece* pie3 = new Piece(1,ps2,app,selecteda);

	pie3->setHidden(0);

	SquarePieces2.push_back(pie3);
	}*/
}

void Board::generateBoard()
{

	//generate lines with pieces
	vector<Piece *> line0 = buildRow(2,"x");
	vector<Piece *> line1 = buildRow(2,"o");
	vector<Piece *> line6 = buildRow(1,"x");
	vector<Piece *> line7 = buildRow(1,"o");

	vector<Piece *> middle;

	//empty lines
	for (int i = 0; i < SIZE; i++)
		middle.push_back(NULL);

	board.push_back(line0);
	board.push_back(line1);
	for (int i = 0; i < SIZE - 4; i++)
		board.push_back(middle);

	board.push_back(line6);
	board.push_back(line7);

}

vector<Piece *> Board::buildRow(int player, string odd)
{
	vector<Piece *> pieces;
	CGFappearance * app;

	/*if(player == 1)
	app = appearanceWhite;
	else
	app = appearanceBlack;*/

	for(int i = 0; i < SIZE; i++)
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
		pieces.push_back(new Piece(player,base,thePiecesApp));
	}

	return pieces;
}

void Board::draw()
{
	for (int i = 0 ; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			glPushMatrix();
			glTranslatef(BASE_X + HALF_LENGTH * j,BASE_Y,BASE_Z + HALF_LENGTH * i);
			if(board[i][j] != NULL)
				board[i][j]->draw();
			glPopMatrix();
		}
	}

	for(int i = 1; i <= 8; i++)
	{
		bool end = true;
		if(nrCircles1 >= i)
		{
			end = false;
			glPushMatrix();
			glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*(i-1),BASE_Z2+0.9);
			circlePiece2->draw();
			glPopMatrix();
		}
		if(nrSquares1 >= i)
		{
			end = false;
			glPushMatrix();
			glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*(i-1),BASE_Z2+0.7);
			squarePiece2->draw();
			glPopMatrix();
		}
		if(nrCircles2 >= i)
		{
			end = false;
			glPushMatrix();
			glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*(i-1),BASE_Z2-0.2);
			circlePiece1->draw();
			glPopMatrix();
		}
		if(nrSquares2 >= i)
		{
			end = false;
			glPushMatrix();
			glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*(i-1),BASE_Z2);
			squarePiece1->draw();
			glPopMatrix();
		}
	}
	/*
	for(int i = 0; i < 8; i++)
	{
	glPushMatrix();
	glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*i,BASE_Z2+0.9);
	if(CirclePieces1[i] != NULL)
	CirclePieces1[i]->draw();
	glPopMatrix();
	//printf("hidden: %d\n",CirclePieces1[i]->getHidden());
	}



	for(int i = 0; i < 8; i++)
	{
	glPushMatrix();
	glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*i,BASE_Z2+0.7);
	if(SquarePieces1[i] != NULL)
	SquarePieces1[i]->draw();
	glPopMatrix();
	}


	for(int i = 0; i < 8; i++)
	{
	glPushMatrix();
	glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*i,BASE_Z2-0.2);
	if(CirclePieces2[i] != NULL)
	CirclePieces2[i]->draw();
	glPopMatrix();
	}



	for(int i = 0; i < 8; i++)
	{
	glPushMatrix();
	glTranslatef(BASE_X2,BASE_Y2+HALF_LENGTH2*i,BASE_Z2);
	if(SquarePieces2[i] != NULL)
	SquarePieces2[i]->draw();
	glPopMatrix();
	}*/
}

bool Board::selectPlace(int y, int x, int player)
{
	/*if(selectedPiece != NULL && board[x][y] == NULL)
	{
	board[x][y] = selectedPiece;
	board[selectedX][selectedY] = NULL;
	selectedPiece = NULL;
	//printf("Moving %d %d to %d %d  (%s)\n",selectedX,selectedY,x,y,board[x][y]->getFormattedPiece());
	cout << "Moving " << selectedX << " " << selectedY << " " << x << " " << y << "(" << board[x][y]->getFormattedPiece() << ")" << endl;
	}
	else
	{
	selectedPiece = board[x][y];
	selectedX = x;
	selectedY = y;
	printf("Selecting %d %d\n",x,y);
	}
	return true;*/
	if(!actions.empty())
		return false;

	if (board[x][y] == NULL)
		return false;

	if(board[x][y]->getPlayer() == player || player == 99)
		board[x][y]->select();
	else 
		return false;

	return true;
}

string Board::getFormatted() const
{
	stringstream ss;

	ss << "[";

	for (int i = 0; i < SIZE; i++)
	{
		ss << "[";

		for (int j = 0; j < SIZE; j++)
		{
			if(board[i][j] == NULL)
				ss << "e";
			else
				ss << board[i][j]->getFormattedPiece();

			if(j < SIZE - 1)
				ss << ",";
		}

		ss << "]";
		if(i < SIZE - 1)
			ss << ",";
	}

	ss << "]";
	return ss.str();
}

bool Board::removeSelection(int y, int x)
{
	board[x][y]->unselect();
	return true;
}

void Board::move(int y1, int x1, int y2, int x2)
{
	//saves move in the stack
	saveMove(x1,y1,x2,y2);

	//adds action to the queue
	addAction(y1,x1,y2,x2);
}

void Board::saveMove(int x1,int y1, int x2, int y2)
{
	Piece * p3 = NULL;
	Piece * p4 = NULL;

	if(board[x1][y1] != NULL)
		p3 = new Piece(*board[x1][y1]);
	else
		p3 = NULL;

	if(board[x2][y2] != NULL)
		p4 = new Piece(*board[x2][y2]);
	else
		p4 = NULL;

	Move * m = new Move(x1,y1,x2,y2,p3,p4);
	moves.push(m);
}


void Board::addAction(int x1,int y1, int x2, int y2)
{

	Action * a = new Action(x1, y1, x2, y2,board[y1][x1], board[y2][x2]);
	actions.push(a);
}

bool Board::pop()
{
	if (moves.empty())
		return false;
	if(!actions.empty())
		return false;

	Move * m = moves.top();
	int x1,y1,x2,y2;

	Piece * p1;
	Piece * p2;

	p1 = m->getOrigin(x1,y1);
	p2 = m->getDestinantion(x2,y2);


	if (p1 != NULL) {
		p1->unselect();
	}
	if(p2 != NULL)
	{
		p2->unselect();
		int c1 = 0, s1 = 0, c2 = 0, s2 = 0;
		p2->getNrPieces(c1,s1,c2,s2);
		nrCircles1 -= c1;
		nrSquares1 -= s1;
		nrCircles2 -= c2;
		nrSquares2 -= s2;
	}

	board[x1][y1] = p1;
	board[x2][y2] = p2;


	moves.pop();

	return true;
}

void Board::playMovie()
{
	board.clear();
	generateBoard();
	stack<Move *> tmp = moves;
	stack<Move *> tmp2;

	nrSquares1 = 0;
	nrSquares2 = 0;
	nrCircles1 = 0;
	nrCircles2 = 0;

	while (!tmp.empty())
	{
		tmp2.push(tmp.top());
		tmp.pop();
	}
	while(!tmp2.empty())
	{
		Move * m = tmp2.top();
		int x1,y1,x2,y2;
		Piece * p1 = m->getOrigin(x1,y1);
		Piece * p2 = m->getDestinantion(x2,y2);

		Action * a = new Action(y1,x1,y2,x2,p1,p2);
		actions.push(a);

		tmp2.pop();
	}
}


bool Board::performAction(unsigned long t)
{
	if(actions.empty())
		return false;

	int x1,x2,y1,y2;
	Action * a = actions.front();
	a->getCoords(y1,x1,y2,x2);


	if(!a->hasStarted())
	{
		a->start();
		board[x1][y1] = a->getPiece(1);
	}

	if(a->getAlmostFinished() && !a->getHandled())
	{
		Piece * p = a->getPiece(1);

		int eaten = 0;

		p->unselect();
		board[x2][y2] = a->getPiece(3);

		//update eaten pieces
		a->updateBoardEaten(nrCircles1, nrSquares1, nrCircles2, nrSquares2);

		board[x1][y1] = NULL;
		a->setHandled();
	}
	else 
		actions.front()->update(t);

	if(a->hasFinished() && a->getHandled())
	{
		camera->setPlayer(actions.front()->getPiece(1)->getPlayer() % 2 +1);
		actions.pop();
	}

	return true;
}

void Board::setCamera(MyMobileCamera * camera)
{
	this->camera = camera;
}

bool Board::getReady() const
{
	return actions.empty();
}
