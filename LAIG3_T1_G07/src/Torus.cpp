#include "Torus.h"
#include <cmath>

/*
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 */


Torus::Torus(float i1,float o1,int sl1,int l1)
{
	inner = i1;
	outer = o1;
	//on purpose after seeing how glutTorus work
	slices = l1;
	loops = sl1;
	/*slices = sl1;
	loops = l1;*/

	TextureWrapVert=1;
	TextureWrapHoriz=1;
	PI2 = 2.0f*3.1415926535;
}

void Torus::putVertTexture(int i, int j) 
{
	float wrapFrac = (j%loops)/(float)loops;
	float wrapFracTex = (float)j/(float)loops;
	float phi = PI2*wrapFrac;
	float thetaFrac = ((float)(i%slices)+wrapFracTex)/(float)slices;
	float thetaFracTex = ((float)i+wrapFracTex)/(float)slices;
	float theta = PI2*thetaFrac;
	float sinphi = sin(phi);
	float cosphi = cos(phi);
	float sintheta = sin(theta);
	float costheta = cos(theta);
	float y = inner*sinphi;
	float r = outer + inner*cosphi;
	float x = sintheta*r;
	float z = costheta*r;

	glTexCoord2f( wrapFracTex*TextureWrapVert, thetaFracTex*TextureWrapHoriz );
	glNormal3f(sintheta*cosphi, sinphi, costheta*cosphi);
	glVertex3f(x,y,z);
}

void Torus::draw()
{
	//glutSolidTorus(inner, outer, slices, loops);
	glPushMatrix();
	glRotatef(90,1,0,0);
	for (int i=0; i<slices; i++ ) {
		glBegin( 1 ? GL_QUAD_STRIP : GL_TRIANGLE_STRIP); 
		for (int j=0; j<=loops; j++) {
			putVertTexture(i, j);
			putVertTexture(i+1,j);
		}
		glEnd();
	}
	glPopMatrix();
}
