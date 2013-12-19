#ifndef _GAME1_H_
#define _GAME1_H_


#include <string>
#include <sstream>
#include <iostream>

#include "Socket.h"
#include "Board.h"
#include "CGFappearance.h"
#include "MyAppearance.h"




class Game
{
private:
	Board * board;
	Socket * socket;

	pair<int,int> selectedPos;
	bool selected;

	CGFappearance * blackAppearance;
	CGFappearance * whiteAppearance;
	void initSocket();
public:
	Game(CGFappearance * black, CGFappearance * white);
	//socket
	Socket * getSocket() const;
	//board
	void initBoard();
	Board * getBoard();
	void setAppearances(CGFappearance * black, CGFappearance * white);
	void draw();
	bool selectPosition(int x, int y);
	bool move(int player, int x, int y);
	bool isValidMove(int x, int y);
	void endGame();
};

#endif