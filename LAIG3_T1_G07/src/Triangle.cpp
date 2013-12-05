#include "Triangle.h"
#include "SurfaceNormal.h"

using std::vector;

Triangle::Triangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3, CGFscene * scene)
{
	vector < vector<float> > vertexes;
	vector < float > vertexA;
	vector < float > vertexB;
	vector < float > vertexC;

	this->x1 = x1;
	vertexA.push_back(x1);
	this->y1 = y1;
	vertexA.push_back(y1);
	this->z1 = z1;
	vertexA.push_back(z1);
	this->x2 = x2;
	vertexB.push_back(x2);
	this->y2 = y2;
	vertexB.push_back(y2);
	this->z2 = z2;
	vertexB.push_back(z2);
	this->x3 = x3;
	vertexC.push_back(x3);
	this->y3 = y3;
	vertexC.push_back(y3);
	this->z3 = z3;
	vertexC.push_back(z3);
	
	vertexes.push_back(vertexA);
	vertexes.push_back(vertexB);
	vertexes.push_back(vertexC);

	normal = CalculateSurfaceNormal(vertexes);
	this->scene = scene;
	
	a = sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2) + (z3-z2)*(z3-z2));
	b = sqrt((x3-x1)*(x3-x1) + (y3-y1)*(y3-y1) + (z3-z1)*(z3-z1));
	c = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
	cosb = (a*a + c*c - b*b)/(2*a*c);
	s3 = c-a*cosb;
	t3 = a * sqrt(1 - cosb*cosb); 
}

void Triangle::draw()
{
	Data * mydata = (((YAFScene*) scene)->getData());
	pair<float,float> coord = mydata->getCurrentTextureCoord();

	glBegin(GL_TRIANGLES);
		glNormal3f(normal[0],normal[1],normal[2]);
		glTexCoord2d(0,0);
		glVertex3f(x1,y1,z1);

		glNormal3f(normal[0],normal[1],normal[2]);
		glTexCoord2d(c/coord.first,0);
		glVertex3f(x2, y2, z2);

		glNormal3f(normal[0],normal[1],normal[2]);
		glTexCoord2d(s3/coord.first,t3/coord.second);
		glVertex3f(x3, y3, z3);
	glEnd();
}