#include "Action.h"

#define BASE_X 11.14375
#define BASE_Y 2.125
#define BASE_Z 11.09375
#define HALF_LENGTH 0.1875

Action::Action(int x1,int y1,int x2, int y2, Piece * piece)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->piece = piece;

	started = false;
	finished = false;

	generateAnimation();
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
	p2.push_back(BASE_Y + 0.5);
	p2.push_back(BASE_Z + HALF_LENGTH * y1);

	p3.push_back(BASE_X + HALF_LENGTH * x2); 
	p3.push_back(BASE_Y + 0.5);
	p3.push_back(BASE_Z + HALF_LENGTH * y2);

	p4.push_back(BASE_X + HALF_LENGTH * x2); 
	p4.push_back(BASE_Y);
	p4.push_back(BASE_Z + HALF_LENGTH * y2);

	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p4);

	animation = new LinearAnimation(points,2);
	start();
}


void Action::start()
{
	started = true;
	piece->setAnimation(animation);
}

void Action::update(double t)
{
	if(animation->isFinished())
	{
		finished = true;
		piece->setAnimation(NULL);
	}
	animation->update(t);
}

bool Action::isFinished() const
{
	return finished;
}