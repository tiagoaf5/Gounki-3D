#include "myClockHand.h"


myClockHand::myClockHand(float tamanho) {
	ponteiro = new Cylinder(0.5,0.5,0.5,30,30);
	this->tamanho = tamanho;
	angle = 0;
	float a[] = {0,0,0,1};
	float b[] = {0,0,0,1};
	float c[] = {0,0,0,1};
	float d[] = {0,0,0,1};
	pont_app = new MyAppearance(a,b,c,d,1);
}

void myClockHand::draw() {
	((MyAppearance *)pont_app)->applyMyAppearance();
	glPushMatrix();
	glRotatef(-angle,0,0,1);
	glRotatef(-90,1,0,0);
	glScalef(0.035,0.035,tamanho);
	ponteiro->draw();
	glPopMatrix();

}

void myClockHand::setAngle(float ang)
{
	angle = ang;
}