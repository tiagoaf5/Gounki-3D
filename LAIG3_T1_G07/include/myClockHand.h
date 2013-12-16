#ifndef MYCLOCKHAND_H
#define MYCLOCKHAND_H

#include "CGFobject.h"
#include "Cylinder.h"
#include "YAFScene.h"
#include "MyAppearance.h"



class myClockHand: public CGFobject {
private:
	CGFobject * ponteiro;
	CGFappearance* pont_app;
	float angle;
	float tamanho; //tamanho ponteiro
	
public:
	myClockHand(float tamanho);
	void draw();
	void setAngle(float ang);
};

#endif