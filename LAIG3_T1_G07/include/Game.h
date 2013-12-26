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

	int activePlayer;
	bool started;
	bool endOfGame;
	int difficulty;  // 1 easy, 2 medium , 3 hard
	int mode; // 1 P V P, 2 P V C , 3 C V P, 4 C V C 

	CGFappearance * blackAppearance;
	CGFappearance * whiteAppearance;
	CGFappearance * selectedAppearance;

	void initSocket();
	bool select(int x, int y);
	bool handleSelection( int x, int y);
	void getPcMove(int &x1, int &y1, int &x2, int &y2);
	void saveMove(int x1,int y1, int x2, int y2);
	int checkEndofGame();
public:
	Game(CGFappearance * black, CGFappearance * white, CGFappearance * selecteda);
	//socket
	Socket * getSocket() const;
	//board
	void initBoard();
	Board * getBoard();
	void setAppearances(CGFappearance * black, CGFappearance * white);
	void draw();
	bool play(int x, int y);
	bool move(int player, int x, int y);
	bool isValidMove(int x, int y);
	void endGame();
	void setDificulty(int difficulty);
	void setMode(int mode);
	void startGame();

	//game modes
	bool pVp(int x, int y);
	bool pVc(int x, int y);
	bool cVp(int x, int y);
	bool cVc();
	bool pop();
};

#endif