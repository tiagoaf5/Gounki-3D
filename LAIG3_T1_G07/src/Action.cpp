#include "Action.h"

#define BASE_X 11.14375
#define BASE_Y 2.125
#define BASE_Z 11.09375
#define HALF_LENGTH 0.1875

Action::Action(int x1,int y1,int x2, int y2, Piece * p1, Piece * p2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->piece1 = p1;
	this->piece2 = p2;

	nrSquares1 = 0;
	nrSquares2 = 0;
	nrCircles1 = 0;
	nrCircles2 = 0;

	started = false;
	finished = false;
	handled = false;
	almostFinished = false;
	animation = NULL;
	result = new Piece(*piece1);
	result->unselect();

	if(p2 != NULL)
		if(p2->getPlayer() == p1->getPlayer())
			result->addPieces(p2->getPieces());


	generateAnimation();
	processEaten();
}

void Action::generateAnimation()
{
	/*

	B ------- C
	|		  |
	A         D

	*/
	vector<vector<double>> points;
	vector<double> p1; //A
	vector<double> p2; //B
	vector<double> p3; //C
	vector<double> p4; //D
	 
	

	p1.push_back(BASE_X + HALF_LENGTH * x1); 
	p1.push_back(BASE_Y);
	p1.push_back(BASE_Z + HALF_LENGTH * y1); 

	p2.push_back(BASE_X + HALF_LENGTH * x1); 
	p2.push_back(BASE_Y + 0.3);
	p2.push_back(BASE_Z + HALF_LENGTH * y1);

	p3.push_back(BASE_X + HALF_LENGTH * x2); 
	p3.push_back(BASE_Y + 0.3);
	p3.push_back(BASE_Z + HALF_LENGTH * y2);

	p4.push_back(BASE_X + HALF_LENGTH * x2); 
	p4.push_back(BASE_Y);
	p4.push_back(BASE_Z + HALF_LENGTH * y2);

	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p4);

	animation = new LinearAnimation(points,2);
	//start();
}


void Action::start()
{
	started = true;
	piece1->setAnimation(animation);
}

void Action::update(unsigned long t)
{
	//printf(".");

	if(animation->getAlmostFinished())
		almostFinished = true;

	if(animation->isFinished())
	{
		almostFinished = true;
		finished = true;
		piece1->setAnimation(NULL);
	}
	else
	{//added
		if(!animation->update(t))
		{
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			almostFinished = true;
			finished = true;
			piece1->setAnimation(NULL);
		}
		//else
		//	printf("::::::::::::::::::::::::::::::::::::");
	}
}

bool Action::hasFinished() const
{
	return finished;
}

bool Action::hasStarted() const
{
	return started;
}

void Action::getCoords(int &x1,int &y1, int &x2, int &y2) const
{
	x1 = this->x1;
	y1 = this->y1;
	x2 = this->x2;
	y2 = this->y2;
}

Piece * Action::getPiece(int i) {
	switch (i)
	{
	case 1:
		return piece1;
	case 2:
		return piece2;
	case 3:
		return result;
	default:
		return NULL;
	}
}

bool Action::getAlmostFinished() {
	return almostFinished;
}

bool Action::getHandled() const { 
	return handled; 
}
void Action::setHandled() { 
	handled = true; 
}

void Action::processEaten()
{
	if(piece2 == NULL)
		return;

	if(piece2->getPlayer() == piece1->getPlayer())
		return;

	piece2->getNrPieces(nrCircles1, nrSquares1, nrCircles2, nrSquares2);
	/*
	string eaten = piece2->getFormattedPiece();
	string temp = eaten;

	if (!eaten.empty())
	{
		if (eaten.find("2") != -1)
		{
			while(temp.find("x") != -1)
			{
				nrSquares1++;
				temp = temp.substr(temp.find("x")+1);
			}
			while(temp.find("o") != -1)
			{
				nrCircles1++;
				temp = temp.substr(temp.find("o")+1);
			}
		}
		else
		{
			while(temp.find("x") != -1)
			{
				nrSquares2++;
				temp = temp.substr(temp.find("x")+1);
			}
			while(temp.find("o") != -1)
			{
				nrCircles2++;
				temp = temp.substr(temp.find("o")+1);
			}
		}
	}*/
}

void Action::updateBoardEaten(int &c1, int &s1, int &c2, int &s2)
{
	c1 += nrCircles1;
	s1 += nrSquares1;
	c2 += nrCircles2;
	s2 += nrSquares2;
}