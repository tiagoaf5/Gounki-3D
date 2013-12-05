#include "Vehicle.h"
#include <cmath>
#include "Data.h"

Vehicle::Vehicle(CGFscene * scene)
{
	

	vector<float>pontos;
	vector<vector<float>> ctrlp;

	body_tex = new CGFtexture("../data/body1.png");
	head_tex = new CGFtexture("../data/head2.png");
	eye_tex = new CGFtexture("../data/eye2.png");

	body = new Sphere(1,30,30);
	head = new Sphere(0.75,30,30);
	eye = new Sphere(0.25,30,30);

	//1
	pontos.push_back(-2.5);
	pontos.push_back(2.75);
	pontos.push_back(0.0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//2
	pontos.push_back(-1.25);
	pontos.push_back(3.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//3
	pontos.push_back(1.25);
	pontos.push_back(3.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//4
	pontos.push_back(2.5);
	pontos.push_back(2.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//5
	pontos.push_back(-2);
	pontos.push_back(0.25);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//6
	pontos.push_back(-1.0);
	pontos.push_back(0.25);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//7
	pontos.push_back(1.0);
	pontos.push_back(0.25);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//8
	pontos.push_back(2);
	pontos.push_back(0.25);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//9
	pontos.push_back(-1.5);
	pontos.push_back(-1.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//10
	pontos.push_back(-0.75);
	pontos.push_back(-1.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//11
	pontos.push_back(0.75);
	pontos.push_back(-1.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//12
	pontos.push_back(1.5);
	pontos.push_back(-1.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//13
	pontos.push_back(-1);
	pontos.push_back(-3.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//14
	pontos.push_back(-0.25);
	pontos.push_back(-3.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//15
	pontos.push_back(0.25);
	pontos.push_back(-3.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();
	//16
	pontos.push_back(1);
	pontos.push_back(-3.75);
	pontos.push_back(0);
	ctrlp.push_back(pontos);
	pontos.clear();

	wing = new Patch(3,20,20,"fill",ctrlp,scene);
}

void Vehicle::draw()
{
	/*glEnable(GL_TEXTURE_2D);
	CGFtexture * b = new CGFtexture("../data/ola.png");
	b->apply();*/
	glEnable(GL_TEXTURE_2D);
	body_tex->apply();
	glPushMatrix();

	glRotatef(90,0,1,0);
	glScalef(0.20,0.20,0.20);

	glPushMatrix();
		glTranslatef(0,1,0);
	glPushMatrix();
		glRotatef(90,0,1,0);
		glScalef(1.0,1.0,2.0);
		body->draw();
	glPopMatrix();
	head_tex->apply();
	glPushMatrix();
		glTranslatef(-2.5,0,0);
		head->draw();
	glPopMatrix();

	eye_tex->apply();

	glPushMatrix();
		glTranslatef(-3.20,0,-0.325);
		glRotatef(-90,0,1,0);
		eye->draw();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-3.20,0,0.25);
		glRotatef(-90,0,1,0);
		glRotatef(-115,0,0,1);
		eye->draw();
	glPopMatrix();

	/*CGFtexture * b1 = new CGFtexture("../data/abc2.png");
	b1->apply();*/

	// ASA 1
	glPushMatrix();
		glScalef(0.35,0.35,0.35);
		glTranslatef(-0.75,3,-4);
		glRotatef(-50,1,0,0);
		glPushMatrix();
			//glScalef(0.5,0.5,0.5);
			//glTranslatef(
			wing->draw();
		glPopMatrix();

		glPushMatrix();
			//glScalef(0.5,0.5,0.5);
			glRotatef(180,0,1,0);
			//glTranslatef(
			wing->draw();
		glPopMatrix();
	glPopMatrix();

	//ASA 2
	glPushMatrix();
		glScalef(0.35,0.35,0.35);
		glTranslatef(-0.75,3,4);
		glRotatef(50,1,0,0);
		glPushMatrix();
			//glScalef(0.5,0.5,0.5);
			//glTranslatef(
			wing->draw();
		glPopMatrix();

		glPushMatrix();
			//glScalef(0.5,0.5,0.5);
			glRotatef(180,0,1,0);
			//glTranslatef(
			wing->draw();
		glPopMatrix();
	glPopMatrix();

	//ferrao
	glPushMatrix();
		//glScalef(0,0,2);
		glTranslatef(1.8,0,0);
		glRotatef(90,0,1,0);
		glutSolidCone(0.25, 1.25,  30, 30);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
