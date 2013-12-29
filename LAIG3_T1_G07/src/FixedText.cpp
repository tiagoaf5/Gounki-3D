#include "FixedText.h"
#include "CGFapplication.h"

FixedText::FixedText(string text, float color[3])
{
	this->text = text;
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
	font = GLUT_BITMAP_HELVETICA_18;
	visible = false;
}

void FixedText::begin2D(bool withDepthTest)
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// push the current matrix as we need it later to restore the perspective view
	glPushMatrix();
	// reset the matrix as we must not multiply it with the new projection
	glLoadIdentity();
	glOrtho( 0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), 0, 1 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(!withDepthTest)
		//disable the depth test, so everything will now be painted on top
		glDisable(GL_DEPTH_TEST);
}

void FixedText::end2D()
{
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	// pop the old matrix which should be the right one if begin2D() is closed by end2D()
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void FixedText::draw()
{
	if(!visible)
		return;

	begin2D(true);
	glEnable(GL_COLOR);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE);

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(color[0], color[1], color[2]);

	glRasterPos2i(glutGet(GLUT_WINDOW_WIDTH)/2 - glutBitmapLength(font,(const unsigned char *)text.c_str())/2, 20);

	glPushMatrix();

	for (std::string::iterator i = text.begin(); i != text.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
	glPopMatrix();
	end2D();
	glPopAttrib();
	glEnable(GL_TEXTURE);
	glEnable(GL_LIGHTING);
	glDisable(GL_COLOR);
}

void FixedText::setText(string t) {
	text = t;
}

void FixedText::hide() {
	visible = false;
}
void FixedText::show() {
	visible = true;
}