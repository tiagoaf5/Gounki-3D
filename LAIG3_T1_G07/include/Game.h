#ifndef _GAME1_H_
#define _GAME1_H_


#include <string>
#include <sstream>
#include <vector>


#include "Socket.h"
#include "Board.h"
#include "CGFappearance.h"
#include "MyAppearance.h"
#include "MyMobileCamera.h"

#include "CGFobject.h"

#include "FixedText.h"
#include "PiecesApperances.h"

class Game
{
private:
	//Objects needed for game
	MyMobileCamera * camera;
	Board * board;
	Socket * socket;
	FixedText * text; //text that says who won
	CGFobject * clock;
	CGFobject * score;

	//current selection
	pair<int,int> selectedPos;
	bool selected;

	//Game status
	bool started;
	bool endOfGame;
	int mode;  // 1 P V P, 2 P V C , 3 C V P, 4 C V C 
	int score1;
	int score2;
	int winner;
	int activePlayer;

	//PC's difficulties 1 easy, 2 hard
	int difficulty1;  
	int difficulty2; 

	//Appearances needed
	PieceAppearances * piecesAppearances;
	vector<CGFappearance *> numberTextures;

	//private functions
	void initSocket();
	void initBoard();
	bool select(int x, int y);
	bool handleSelection( int x, int y);
	void getPcMove(int &x1, int &y1, int &x2, int &y2);
	int checkEndofGame();
	void changePlayer();
	void processWinner(bool update); //true to move on, false to move back
	bool isValidMove(int x, int y);

	//game modes
	bool pVp(int x, int y);
	bool pVc(int x, int y);
	bool cVp(int x, int y);
	bool cVc();

public:
	Game(CGFappearance * black, CGFappearance * white, CGFappearance * selecteda);

	//board
	Board * getBoard();

	//appearances
	void setAppearances(CGFappearance * black, CGFappearance * white);
	void setNumberAppearances(vector<CGFappearance *> numberTextures);

	//display actions
	void draw();
	void drawText();

	//Playing actions
	void startGame();
	bool play(int x, int y);
	void endGame();
	bool pop();
	void playMovie();

	//Setting game options
	void setDificulty(int difficulty1, int difficulty2);
	void setMode(int mode);

	//camera
	void setCamera(MyMobileCamera * camera);

	//clock
	void setClock(CGFobject * clock);

	//updates the game
	void update (unsigned long t);
};

#endif