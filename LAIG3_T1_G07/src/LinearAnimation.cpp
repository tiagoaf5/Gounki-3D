#include "LinearAnimation.h"

#include <iostream>

using namespace std;

const double PI=3.141592653589793238462;

LinearAnimation::LinearAnimation(vector<vector<double>> points, double span): Animation()
{
	this->points = points;
	this->span = span * 1000;
	almostFinished = false;
	//this->span = span;
	init();
}


void LinearAnimation::init()
{

	for (unsigned int i = 0; i < points.size() - 1; i++)
	{
		double lenght;
		tangents.push_back(getTangent(points[i],points[i+1], &lenght));
		distances.push_back(lenght);
	}

	double totalLength = 0;

	for(unsigned int i = 0; i < distances.size(); i++)
	{
		totalLength += distances[i];
	}
	double acumulator = 0.0;
	for(unsigned int i = 0; i < distances.size(); i++)
	{
		double abc = (distances[i] / totalLength) * span;
		acumulator+= abc;
		time.push_back(acumulator);
	}

	speed = totalLength / span;
/*
	printf("Animation details:\nPoints:\n");
	for(unsigned int i = 0; i < points.size(); i++)
		printf("%i	(%f,%f,%f)\n",i,points[i][0],points[i][1],points[i][2]);

	printf("\nTangents:\n");
	for(unsigned int i = 0; i < tangents.size(); i++)
		printf("%i	(%f,%f,%f)\n",i,tangents[i][0],tangents[i][1],tangents[i][2]);

	printf("\nDistances:\n");
	for(unsigned int i = 0; i < distances.size(); i++)
		printf("%i	%f\n",i,distances[i]);

	printf("\nTotalLength: %f\n",totalLength);

	printf("\nTimes:\n");
	for(unsigned int i = 0; i < time.size(); i++)
		printf("%i	%f\n",i,time[i]);

	printf("\nSpeed: %f\n",speed);*/
}

vector<double> LinearAnimation::getTangent(vector<double> p1, vector<double> p2, double *length)
{
	vector<double> tg;

	tg.push_back(p2[0] - p1[0]);
	tg.push_back(p2[1] - p1[1]);
	tg.push_back(p2[2] - p1[2]);

	*length = sqrt(tg[0] * tg[0] + tg[1] * tg[1] + tg[2] * tg[2]);

	tg[0] /= *length;
	tg[1] /= *length;
	tg[2] /= *length;

	return tg;
}

void LinearAnimation::handlerUpdate(unsigned long animT)
{
	if(animT >= span)
	{
		finished = true;
		return;
	}
	if(animT >= span - 150) //100
		almostFinished = true;

	unsigned int i;

	for(i = 0; i < time.size();i++)
		if(animT < time[i])
			break;

	//to abstract a little
	double x = points[i][0];
	double y = points[i][1];
	double z = points[i][2];

	double tx = tangents[i][0];
	double ty = tangents[i][1];
	double tz = tangents[i][2];

	double i_time;

	if(i > 0)
		i_time = animT - time[i-1];
	else
		i_time = animT;


	double deltax = x + i_time * speed * tx;
	double deltay = y + i_time * speed * ty;
	double deltaz = z + i_time * speed * tz;

	double alfa;

	if(tz == 0)
	{
		if(tx < 0)
			alfa = 270.0;
		else if (tx > 0)
			alfa = 90.0;
		else 
			alfa = 0.0;
	}
	else
	{
		/*
		---------
		| 3 | 2 |
		---------
		| 4 | 1 |
		---------
		*/

		alfa = atan(abs(tx / tz)) * 180 / PI; //1st quadrant

		if(tx > 0 && tz < 0) //2nd quadrant
			alfa += 90.0;
		else if(tx <= 0 && tz < 0) //3rd quadrant
			alfa += 180.0;
		else if(tx < 0 && tz > 0) //4th quadrant
			alfa += 270.0;
	}
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(deltax,deltay,deltaz);
	glRotated(alfa,0,1,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, &transforms[0][0]);
	glPopMatrix();
}

bool LinearAnimation::getAlmostFinished() const
{
	return almostFinished;
}