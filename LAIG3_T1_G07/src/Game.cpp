#include "Game.h"

using namespace std;

Game::Game(CGFappearance * black, CGFappearance * white, CGFappearance * selecteda)
{
	blackAppearance = black;
	whiteAppearance = white;
	selectedAppearance = selecteda;
	//initSocket();
	//initBoard();
	
	board = NULL;
	mode = 1;
	difficulty = 1;

	selected = false;
	selectedPos.first = -1;
	selectedPos.second = -1;
	activePlayer = 1;
	started = false;
	endOfGame = false;
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
	board = new Board(blackAppearance,whiteAppearance,selectedAppearance);
}

Board * Game::getBoard()
{
	return board;
}


void Game::setAppearances(CGFappearance * black, CGFappearance * white)
{
	blackAppearance = black;
	whiteAppearance = white;
}


void Game::draw()
{
	if(board != NULL)
		board->draw();
}

bool Game::play(int x, int y)
{
	printf("Mode: %d\n", mode);

	if(!started || endOfGame)
	{
		printf("not started\n");
		return false;
	}
		

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

	checkEndofGame();
	return val;
}

bool Game::isValidMove(int x, int y)
{
	stringstream ss;

	ss << "play(" <<selectedPos.first+1 << "," << selectedPos.second +1 << "," << x +1<< ","<< y +1 << "," << board->getFormatted(1) << ").\n";

	string val = ss.str();
	socket->sendData((char *)val.c_str(), val.size());

	char abc[64];
	socket->receiveData(abc);

	if(strcmp(abc, "ok.") == 0)
		return true;
	else
		return false;
}

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
}

void Game::endGame()
{
	started = false;
	socket->quit();
}

void Game::setDificulty(int difficulty)
{
	this->difficulty = difficulty;
}

void Game::setMode(int mode)
{
	this->mode = mode;

	if(mode == 3 && started)
		cVp(-1,-1);
}

void Game::startGame()
{
	if(!started)
		initSocket();

	initBoard();

	endOfGame = false;
	started = true;
	activePlayer = 1;
	printf("Mode: %d\nDifficulty: %d\n", mode,difficulty);

	if(mode == 3)
		cVp(-1,-1);
}

bool Game::pVp(int x, int y)
{
	printf("<pvp>\n");
	if (!selected)
		return select(x,y);
	else
		return handleSelection(x, y);
}

bool Game::pVc(int x, int y)
{
	int a,b,c,d;
	
	if( x == -1 && y == -1 && activePlayer == 1)
		return false;
	
	if (activePlayer == 1)
	{
		if (!selected)
			return select(x,y);
		else
		{
			if(handleSelection(x, y))
			{
				getPcMove(a,b,c,d);
				printf("move %d %d to %d %d\n",a,b,c,d);
				board->selectPlace(a-1,b-1, activePlayer);
				board->move(a-1,b-1,c-1,d-1);
				activePlayer = activePlayer % 2 + 1;
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
		activePlayer = activePlayer % 2 + 1;
	}

	return true;
}
bool Game::cVp(int x, int y)
{
	if( x == -1 && y == -1 && activePlayer == 2)
		return false;

	int a,b,c,d;


	if (activePlayer == 2)
	{
		if (!selected)
			return select(x,y);
		else
		{
			if(handleSelection(x, y))
				return cVp(-1,-1); // to let the PC play
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
		activePlayer = activePlayer % 2 + 1;
	}

	return true;
}

bool Game::cVc()
{
	int a,b,c,d;

	getPcMove(a,b,c,d);
	printf("move %d %d to %d %d\n",a,b,c,d);
	board->selectPlace(a-1,b-1, activePlayer);
	board->move(a-1,b-1,c-1,d-1);
	activePlayer = activePlayer % 2 + 1;

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
			string eaten = board->move(selectedPos.first, selectedPos.second,x,y);
			theReturn = true;
			//change player
			activePlayer = activePlayer % 2 + 1;
			printf("2. %d\n", activePlayer);
			selected = false;
		}
		else
		{
			selected = false;
			board->removeSelection(x,y); 	//tell board nothing is selected
			theReturn = false;
		}
		
	}
	return theReturn;
}

void Game::getPcMove(int &x1, int &y1, int &x2, int &y2)
{
	stringstream ss;

	ss << "pc_move(" << activePlayer << "," << difficulty << "," << board->getFormatted(1) << ").\n";

	socket->sendData((char *)(ss.str().c_str()), ss.str().size());
	
	cout << "Sent: " << ss.str() << endl;
	char abc[200];
	socket->receiveData(abc);
	sscanf(abc,"%d-%d-%d-%d.",&x1,&y1,&x2,&y2);
}

bool Game::pop()
{
	if(board->pop())
	{
		activePlayer = activePlayer % 2 + 1;
		//play(-1,-1);
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
	printf("--->%s<---\n",abc);
	
	if(strcmp(abc,"no.") != 0)
	{
		printf("Won player %s\n", abc);
		endOfGame = true;
		return true;
	}
	
	return false;
}