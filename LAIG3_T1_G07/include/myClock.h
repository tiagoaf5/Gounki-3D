#ifndef MYCLOCK_H
#define MYCLOCK_H

#include "CGFobject.h"
#include "CGFappearance.h"
#include "Cylinder.h"
#include "YAFScene.h"
#include "myClockHand.h"



class myClock: public CGFobject {
private:
	CGFobject * relogio;
	CGFtexture * relog_tex;
	CGFscene * scene;
	CGFobject *minutos;
	CGFobject *horas;
	CGFobject *segundos;
	unsigned long anterior;
	double segundos_ang;
	double minutos_ang;
	double horas_ang;
	unsigned long startTime;
	int parado; // 0 anda 1 parado
public:
	myClock(CGFscene * scene);
	void draw();
	void update(unsigned long num);
	int getParado();
	void setParado();
	void reset();
	unsigned long getTime() const;
};

#endif