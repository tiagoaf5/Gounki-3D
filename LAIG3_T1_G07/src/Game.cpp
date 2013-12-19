#include "Game.h"

using namespace std;

Game::Game(CGFappearance * black, CGFappearance * white)
{
	blackAppearance = black;
	whiteAppearance = white;
	initSocket();
	initBoard();

	selected = false;
	selectedPos.first = -1;
	selectedPos.second = -1;
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
	board = new Board(blackAppearance,whiteAppearance);
	/*stringstream ss;
	ss << "play(" <<"1,10,1,6," << board->getFormatted() << ").\n";
	string val = ss.str();

	cout << "sent: " << val << endl;


	socket->sendData((char *)val.c_str(), val.size());
	char abc[200];
	socket->receiveData(abc);*/
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
	board->draw();
}

bool Game::selectPosition(int x, int y)
{
	if (!selected)
	{
		if(board->selectPlace(x,y))
		{
			selectedPos.first = x;
			selectedPos.second = y;
			selected = true;
			return true;
		}
		else
			return false;
	}
	else
	{
		if(selectedPos.first == x && selectedPos.second == y)
		{
			selected = false;
			board->removeSelection(x,y); 	//informar board que nada está selecionado
			return false;
		}
		else
		{
			if(isValidMove(x,y))
				board->move(selectedPos.first, selectedPos.second,x,y);

			selected = false;
			//move from position in the pair to the received position
		}
	}
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

	board->selectPlace(x,y);
	return true;
}

void Game::endGame()
{
	socket->quit();
}