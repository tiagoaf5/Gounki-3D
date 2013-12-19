#include "PieceSquare.h"
#include "Plane.h"

using namespace std;

PieceSquare::PieceSquare()
{
	//piece = new Cylinder(0.06,0.06,0.025,20,5);

	piece = new Plane(4,4,NULL);


}

string PieceSquare::getFormattedPiece()
{
	return "x";
}

void PieceSquare::draw()
{
	glPushMatrix();
	glScalef(0.12,0.12,0.12);
	//glRotatef(90,1,0,0);
	piece->draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0,-0.0125,0.06);
	glRotatef(90,1,0,0);
	glScalef(0.12,1,0.025);
	piece->draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.06,-0.0125,0);
	glRotatef(90,0,1,0);
	glRotatef(90,1,0,0);
	glScalef(0.12,1,0.025);
	piece->draw();
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(-0.06,-0.0125,0);
	glRotatef(-90,0,1,0);
	glRotatef(90,1,0,0);
	glScalef(0.12,1,0.025);
	piece->draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0,-0.0125,-0.06);
	glRotatef(-90,1,0,0);
	glScalef(0.12,1,0.025);
	piece->draw();
	glPopMatrix();


}