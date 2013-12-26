#include "Board.h"

#define BASE_X 11.14375
#define BASE_Y 2.125
#define BASE_Z 11.09375
#define HALF_LENGTH 0.1875
#define SIZE 8

//test purpose
#include <iostream>
using namespace std;

Board::Board(CGFappearance * black, CGFappearance * white, CGFappearance * selecteda)
{
	appearanceBlack = black;
	appearanceWhite = white;
	appearanceSelected = selecteda;
	selectedPiece = NULL;
	generateBoard();
	generateBoard(1);
}

void Board::generateBoard(int b)
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
	
	if(b == 0)
	{
		board.push_back(line0);
		board.push_back(line1);
		for (int i = 0; i < SIZE - 4; i++)
			board.push_back(middle);

		board.push_back(line6);
		board.push_back(line7);
	}
	else
	{
		board_aux.push_back(line0);
		board_aux.push_back(line1);
		for (int i = 0; i < SIZE - 4; i++)
			board_aux.push_back(middle);

		board_aux.push_back(line6);
		board_aux.push_back(line7);
	}
}

vector<Piece *> Board::buildRow(int player, string odd)
{
	vector<Piece *> pieces;
	CGFappearance * app;

	if(player == 1)
		app = appearanceWhite;
	else
		app = appearanceBlack;

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
		pieces.push_back(new Piece(player,base,app,appearanceSelected));
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


	if (board[x][y] == NULL)
		return false;
	
	if(board[x][y]->getPlayer() == player)
	{
		selectedPiece = board[x][y];
		selectedPiece->select();
	}
	else 
		return false;

	return true;
}

string Board::getFormatted(int b) const
{
	stringstream ss;
	vector<vector<Piece *>> board1;
	if (b == 0)
		board1 = board;
	else
		board1 = board_aux;

	ss << "[";

	for (int i = 0; i < SIZE; i++)
	{
		ss << "[";

		for (int j = 0; j < SIZE; j++)
		{
			if(board1[i][j] == NULL)
				ss << "e";
			else
				ss << board1[i][j]->getFormattedPiece();

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

bool Board::removeSelection(int y, int x) //need review
{
	selectedPiece->unselect();
	if(board[x][y] == NULL)
		return false;
	else
		board[x][y] == NULL;
	return true;
}

string Board::move(int y1, int x1, int y2, int x2)
{
	//saves move in the stack
	saveMove(x1,y1,x2,y2);
	//adds action to the queue
	addAction(x1,y1,x2,y2);

	string eaten;
	if(board_aux[x2][y2] != NULL)
	{

		if(board_aux[x2][y2]->getPlayer() == board_aux[x1][y1]->getPlayer())
		{
			board_aux[x2][y2]->addPieces(board_aux[x1][y1]->getPieces());
		}
		else
		{
			eaten = board_aux[x2][y2]->getFormattedPiece();
			board_aux[x2][y2] = board_aux[x1][y1];
		}
	}
	else
		board_aux[x2][y2] = board_aux[x1][y1];
	
	board_aux[x1][y1] = NULL;

	
	return eaten;
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

	Piece * p3 = NULL;
	if (p1 != NULL)
		p3 = new Piece(*p1);
	Piece * p4 = NULL; 
	if(p2 != NULL)
		p4 = new Piece(*p2);

	board[x1][y1] = p1;
	board[x2][y2] = p2;

	board_aux[x1][y1] = p3;
	board_aux[x2][y2] = p4;

	moves.pop();

	return true;
}

void Board::addAction(int y1,int x1, int y2, int x2)
{
	Action * a = new Action(x1, y1, x2, y2,selectedPiece);
	actions.push(a);
}

bool Board::performAction(unsigned long t)
{
	if(actions.empty())
		return false;
	
	Action * a = actions.front();
	
	if(!a->hasStarted())
		a->start();

	Piece * p = a->getPiece();
	int x1,x2,y1,y2;
	int eaten = 0;
	int almost = a->getAlmostFinished();
	
	
	a->getCoords(y1,x1,y2,x2);
	printf("%d, ",almost);

	if(almost == 1)
	{
		p->unselect();
		if(board[x2][y2] != NULL)
		{
			if(board[x2][y2]->getPlayer() == p->getPlayer())
			{
				printf("abc\n");
				board[x2][y2]->addPieces(p->getPieces());
			}
			else
			{
				eaten = board[x2][y2]->getPieces().size();
				board[x2][y2] = p;
			}
		}
		else
			board[x2][y2] = p;

		board[x1][y1] = NULL;

		//actions.pop();
	}
	else// if(a->getAlmostFinished() == 0)
	{
		actions.front()->update(t);
	}

	if(a->hasFinished())
		actions.pop();

	return true;
	/*
	if(actions.empty())
		return NULL;
	else 
	{
		if(actions.front()->isFinished())
			actions.pop();
		else
			return actions.front();

		return getAction();
	}*/

}