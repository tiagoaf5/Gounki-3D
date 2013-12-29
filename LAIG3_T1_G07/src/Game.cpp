#include "Game.h"
#include "myClock.h"
#include "MyRectangle.h"
#include "CGFapplication.h"

#define BASE_X 10.9
#define BASE_Y 2.02
#define BASE_Z 11.4
#define HALF_LENGTH 0.03
using namespace std;

Game::Game(CGFappearance * black, CGFappearance * white, CGFappearance * selecteda)
{
	/*blackAppearance = black;
	whiteAppearance = white;
	selectedAppearance = selecteda;*/
	piecesAppearances = new PieceAppearances(white,black,selecteda);
	/*thePiecesApp->black = black;
	thePiecesApp->white = white;
	thePiecesApp->selected = selecteda;*/


	float color[] = {1.0,1.0,1.0};
	text = new FixedText("",color);
	//initSocket();
	//initBoard();

	board = NULL;
	mode = 1;

	difficulty1 = 1;
	difficulty2 = 1;

	selected = false;
	selectedPos.first = -1;
	selectedPos.second = -1;
	activePlayer = 1;
	started = false;
	endOfGame = false;

	score1 = 0;
	score2 = 0;
	winner = 1;

	score = new MyRectangle(-0.1875, -0.375,0.1875, 0.375, NULL);
	((MyRectangle *)score)->setTextLength(0.375,0.75);
}

void Game::initSocket()
{
	socket = new Socket();
}

Socket * Game::getSocket() const
{
	return socket;
}


void Game::initBoard()
{
	board = new Board(piecesAppearances);
	board->setCamera(camera);
}

Board * Game::getBoard()
{
	return board;
}


void Game::setDificulty(int difficulty1, int difficulty2)
{
	this->difficulty1 = difficulty1;
	this->difficulty2 = difficulty2;
}

void Game::setMode(int mode)
{
	this->mode = mode;

	//if(mode == 3 && started)
	//play(-1,-1);
}

void Game::setAppearances(CGFappearance * black, CGFappearance * white)
{
	piecesAppearances->setAppearances(white,black);
}


void Game::draw()
{
	glPushMatrix();

		glPushMatrix();
			
			glTranslatef(12.8, 2, 11.8);
			glRotatef(-90,0,1,0);
			glScalef(0.2, 0.2, 0.2);
			glTranslatef(0.5, 0.5, 0.74);
			/*<translate to="-0.5 0.5 0.74" />
			<translate to ="12.8 2 11.8"/>
			<rotate axis="y" angle ="-90" />
			<scale factor = "0.2 0.2 0.2"/>*/
			
			numberTextures[score1%10]->apply();
			score->draw();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(12.8, 2, 11.8);
			glRotatef(-90,0,1,0);
			glScalef(0.2, 0.2, 0.2);
			glTranslatef(-0.5, 0.5, 0.74);
			numberTextures[score2%10]->apply();
			score->draw();
		glPopMatrix();
	glPopMatrix();


	if(board != NULL)
		board->draw();
}

void Game::drawText() {
	text->draw();
}

bool Game::play(int x, int y)
{
	printf("Mode: %d\n", mode);

	if(!started || endOfGame)
	{
		printf("not started\n");
		return false;
	}

	if(!board->getReady())
		return false;

	if(checkEndofGame() >= 1)
		return true;

	bool val = false;

	switch (mode)
	{
	case 1:
		val = pVp(x,y);
		break;
	case 2:
		val = pVc(x,y);
		break;
	case 3:
		val = cVp(x,y);
		break;
	case 4:
		val = cVc();
		break;
	}

	/*if(val)
	checkEndofGame();*/
	return val;
}

bool Game::isValidMove(int x, int y)
{
	stringstream ss;

	ss << "play(" <<selectedPos.first+1 << "," << selectedPos.second +1 << "," << x +1<< ","<< y +1 << "," << board->getFormatted() << ").\n";

	string val = ss.str();
	socket->sendData((char *)val.c_str(), val.size());

	char abc[64];
	socket->receiveData(abc);

	if(strcmp(abc, "ok.") == 0)
		return true;
	else
		return false;
}
/*
bool Game::move(int player, int x, int y)
{
stringstream ss;
ss << "play(" <<selectedPos.first << "," << selectedPos.second << "," << x << ","<< y << board->getFormatted() << ").\n";
string val = ss.str();

cout << "sent: " << val << endl;


socket->sendData((char *)val.c_str(), val.size());
char abc[200];
socket->receiveData(abc);

if(strcmp(abc,"ok") == 0)
return true;
else 
return false;

board->selectPlace(x,y,player);
return true;
}*/

void Game::endGame()
{
	started = false;
	socket->quit();
}

void Game::startGame()
{
	if(!started)
		initSocket();

	initBoard();

	endOfGame = false;
	started = true;
	activePlayer = 1;
	printf("Mode: %d\nDifficulty: %d %d\n", mode,difficulty1,difficulty2);

	((myClock *)(clock))->reset();
	text->hide();
	/*
	if(mode == 3)
	cVp(-1,-1);*/
}

bool Game::pVp(int x, int y)
{
	if( x == -1 && y == -1)
		return false;

	printf("<pvp>\n");
	if (!selected)
		select(x,y);
	else
		return handleSelection(x, y);

	return false;
}

bool Game::pVc(int x, int y)
{
	int a,b,c,d;

	if( x == -1 && y == -1 && activePlayer == 1)
		return false;

	if (activePlayer == 1)
	{
		if (!selected)
			select(x,y);
		else
		{
			if(handleSelection(x, y))
			{
				/*getPcMove(a,b,c,d);
				printf("move %d %d to %d %d\n",a,b,c,d);
				board->selectPlace(a-1,b-1, activePlayer);
				board->move(a-1,b-1,c-1,d-1);
				activePlayer = activePlayer % 2 + 1;*/
				//return pVc(-1,-1);
			}
			else
				return false;
		}

	}
	else
	{
		getPcMove(a,b,c,d);
		printf("move %d %d to %d %d\n",a,b,c,d);
		board->selectPlace(a-1,b-1, activePlayer);
		board->move(a-1,b-1,c-1,d-1);
		changePlayer();
		return true;
	}

	return false;
}
bool Game::cVp(int x, int y)
{
	if( x == -1 && y == -1 && activePlayer == 2)
		return false;

	int a,b,c,d;


	if (activePlayer == 2)
	{
		if (!selected)
			select(x,y);
		else
		{
			if(handleSelection(x, y))
				return true;//cVp(-1,-1); // to let the PC play
			else
				return false;
		}

	}
	else
	{
		getPcMove(a,b,c,d);
		printf("move %d %d to %d %d\n",a,b,c,d);
		board->selectPlace(a-1,b-1, activePlayer);
		board->move(a-1,b-1,c-1,d-1);
		changePlayer();
		return true;
	}

	return false;
}

bool Game::cVc()
{
	int a,b,c,d;

	getPcMove(a,b,c,d);
	printf("move %d %d to %d %d\n",a,b,c,d);
	board->selectPlace(a-1,b-1, activePlayer);
	board->move(a-1,b-1,c-1,d-1);
	changePlayer();

	return true;
}

bool Game::select(int x, int y)
{
	if(board->selectPlace(x,y, activePlayer))
	{
		selectedPos.first = x;
		selectedPos.second = y;
		selected = true;
		return true;
	}
	else
		return false;
}

bool Game::handleSelection(int x, int y) 
{
	bool theReturn;

	if(selectedPos.first == x && selectedPos.second == y)
	{
		selected = false;
		board->removeSelection(x,y); 	//tell board nothing is selected
		theReturn = false;
	}
	else
	{
		theReturn = false;
		if(isValidMove(x,y))
		{
			board->move(selectedPos.first, selectedPos.second,x,y);
			theReturn = true;
			changePlayer(); //change player
			selected = false;
		}
		else
		{
			/*printf("invalid move %d %d\n",x,y);
			selected = false;
			board->removeSelection(x,y); 	//tell board nothing is selected*/
			theReturn = false;
		}

	}
	return theReturn;
}

void Game::getPcMove(int &x1, int &y1, int &x2, int &y2)
{
	stringstream ss;
	int difficulty = difficulty1;

	if(activePlayer == 2)
		difficulty = difficulty2;

	ss << "pc_move(" << activePlayer << "," << difficulty << "," << board->getFormatted() << ").\n";

	socket->sendData((char *)(ss.str().c_str()), ss.str().size());

	cout << "Sent: " << ss.str() << endl;
	char abc[200];
	socket->receiveData(abc);
	sscanf(abc,"%d-%d-%d-%d.",&x1,&y1,&x2,&y2);
}

bool Game::pop()
{
	if(!started)
		return false;

	if(endOfGame)
	{
		endOfGame = false;
		processWinner(false);
	}

	if(board->pop())
	{
		changePlayer();
		//play(-1,-1);
		camera->setPos(activePlayer);
		return true;
	}

	return false;
}

int Game::checkEndofGame()
{
	stringstream ss;

	ss << "check_end_of_game(" << board->getFormatted() << ").\n";

	socket->sendData((char *)ss.str().c_str(), ss.str().size());
	char abc[100];

	socket->receiveData(abc);

	if(strcmp(abc,"no.") != 0)
	{
		stringstream ss1;
		char abc1[2];

		printf("Won player %s\n", abc);
		endOfGame = true;
		
		abc1[1] = '\0';
		abc1[0] = abc[0];

		int aa = atoi(abc1);

		/*-ss1 << "Player " << aa << " won!";
		cout << "-------------> " << ss1.str() << endl;
		text->setText(ss1.str());
		text->show();
		winner = aa;-*/
		winner = aa;
		processWinner(true);

		return aa;
	}

	return -1;
}

void Game::setCamera(MyMobileCamera * camera)
{
	this->camera = camera;
	if (board != NULL)
		board->setCamera(camera);
}

void Game::playMovie()
{
	if(endOfGame)
		board->playMovie();
}

void Game::setClock(CGFobject * clock)
{
	this->clock = clock;

	((myClock *)(clock))->reset();
}

void Game::changePlayer()
{
	activePlayer = activePlayer % 2 + 1;
	((myClock *)(clock))->reset();
}

void Game::update (unsigned long t)
{
	if(board != NULL)
		board->performAction(t);

	if(((myClock*)clock)->getTime() >= 30 && started && !endOfGame)
	{
		if (selected)
		{
			board->removeSelection(selectedPos.first, selectedPos.second);
			selected = false;
		}
		changePlayer();
		((myClock*)clock)->reset();
		camera->setPos(activePlayer);
	}
}

void Game::setNumberAppearances(vector<CGFappearance *> numberTextures) {
	this->numberTextures = numberTextures;
}


void Game::processWinner(bool update)
{
	int p;
	if(update)
	{
		stringstream ss;

		ss << "Player " << winner << " won!";
		text->setText(ss.str());
		text->show();


		if(winner == 1) {
			score1++;
			p = 1;
		}
		else if (winner == 2) {
			score2++;
			p = 2;
		}
	}
	else
	{
		if(winner == 1) {
			score1--;
			p = 1;
		}
		else if (winner == 2) {
			score2--;
			p = 2;
		}

		text->hide();
	}

	if(score1 < 0 || score1 > 9 || score2 < 0 || score2 > 9)
		return;

	printf("Won player %d\n", winner);
	printf("Score1: %d  Score2: %d\n",score1, score2);

	/*if(p == 1)
		score1Appearance->setTexture(f1);
	else
		score2Appearance->setTexture(f2);*/

}