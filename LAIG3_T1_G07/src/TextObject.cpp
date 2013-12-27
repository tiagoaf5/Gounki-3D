#include "TextObject.h"
#include <GL/glut.h>

TextObject::TextObject(int type){
	this->type = type;

	if (type == 1){
		posX = 12.54;
		posY = 2.13;
		posZ = 11.82;
	}
	else{
		posX = 12.54;
		posY = 2.13;
		posZ = 11.60;
 	}
}

void TextObject::draw()
{
	// Texto 3D, GLUT
	// void glutStrokeCharacter(void *font, int character);	// GLUT_STROKE_ROMAN
	// int glutStrokeWidth(GLUTstrokeFont font, int character);

	// desativa cálculo de iluminação para usar cores diretamente
	glDisable(GL_LIGHTING);

	// Texto BitMap, GLUT
	// glRasterPos3f(x,y,z);
	// void glutBitmapCharacter(void *font, int character);	// valores varios...
	// int glutBitmapWidth(GLUTbitmapFont font, int character);

	//printf("NUMBER: %c\n",number);

	glPushMatrix();
	glTranslatef(posX,posY,posZ);
	glScalef(0.0015, 0.0015, 0.0015);
	glRotatef(-90,0,1,0);
	glColor3f(0.0,0.0,1.0);		// azul
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, number);
	glPopMatrix();


	glEnable(GL_LIGHTING);

}

void TextObject::setNumber(char number){
	this->number = number;
}