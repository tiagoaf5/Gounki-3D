#include "myClock.h"

// Coefficients for material B
float amb[3] = {0.2, 0.2, 0.2};
float dif[3] = {0.5, 0.5, 0.5};
float spec[3] = {0.6, 0.6, 0.6};
float shininess = 120.f;

float ambs[3] = {0.2, 0.2, 0.2};
float difs[3] = {0, 0, 0};
float specs[3] = {0, 0, 0};
float shininesss = 120.f;





myClock::myClock(CGFscene * scene)
{
	this->scene = scene;
	relogio = new Cylinder(0.5,0.5,0.10,30,30);
	relog_tex = new CGFtexture("../data/clock.png");
	//materials = new CGFappearance(ambs,difs,specs,shininesss);
	segundos = new myClockHand(0.9);
	//minutos = new myClockHand(0.67);
	//horas = new myClockHand(0.5);

	segundos_ang = 0;
	//minutos_ang = 180;
	//horas_ang = 90;
	anterior = 0;
	parado = 0 ;
}

void myClock::draw() {
	//glEnable(GL_TEXTURE_2D);

	//relog_tex->apply();

	glPushMatrix();
	relogio->draw();
	glPopMatrix();
	

	glPushMatrix();
	glTranslated(0,0,0.12);
	((myClockHand *)segundos)->setAngle(segundos_ang);
	segundos->draw();
	glPopMatrix();

	/*glPushMatrix();
	glTranslated(0,0,0.12);
	glRotatef(40,0,0,1);
	((myClockHand *)minutos)->setAngle(minutos_ang);
	minutos->draw();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(0,0,0.12);
	glRotatef(-40,0,0,1);
	((myClockHand *)horas)->setAngle(horas_ang);
	horas->draw();
	glPopMatrix();*/
}

void myClock::update(unsigned long num)
{
	if (parado == 0){
	
		if (anterior == 0)
			anterior = num;
		else
		{
			double s = (num - anterior) / 1000.0 * 360 / 60.0;
			segundos_ang += s;
			/*double m = (num - anterior) / 1000.0 * 360 / 3600.0;
			minutos_ang += m;
			double h = (num - anterior) / 1000.0 / 120.0;
			horas_ang += h;*/
		}
		anterior = num;
	}
}

int myClock::getParado(){
	return parado;
}

void myClock::reset(){
	segundos_ang = 0;

}

void myClock::setParado(){
	if (parado == 1){
		parado = 0;
		anterior = 0;
		return;
	}
	if (parado == 0){
		parado = 1;
		return;
	}
}
