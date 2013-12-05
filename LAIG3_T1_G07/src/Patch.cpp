#include "Patch.h"
#include "Data.h"

Patch::Patch(int order,int partsU,int partsV,string compute,vector<vector<float> > controlPoints ,CGFscene * scene){

	this->order = order;
	this->partsU = partsU;
	this->partsV = partsV;
	this->compute = compute;
	this->controlPoints = controlPoints;
	this->scene = scene;

	this->nrControlPoints = (order+1)*(order+1);

	

	for (int i = 0; i < nrControlPoints; i++){
		for (int j = 0;  j < 3; j++){
			ctrlpoints[i][j] = controlPoints[i][j];
		}
	}

	if(compute != "fill" && compute != "point" && compute != "line")
	{
		printf("Invalid compute mode for patch\n");
		exit(1);
	}

	unsigned int pos0 = 0;
	unsigned int pos1 = order; // + 1 - 1

	/* approximate patch to a rectangle
	p1 xx xx p2             lengthU |p2-p1|
	xx xx xx xx
	xx xx xx xx 
	xx xx xx p3             lengthV |p2-p3|
	*/
	vector<float> p1 =   controlPoints[pos0];
	vector<float> p2 =   controlPoints[pos1];
	vector<float> p3 =   controlPoints[controlPoints.size() -1];

	lengthU = sqrt((p2[0] - p1[0]) * (p2[0] - p1[0]) + (p2[1] - p1[1]) * (p2[1] - p1[1]) + (p2[2] - p1[2]) * (p2[2] - p1[2]));
	lengthV = sqrt((p2[0] - p3[0]) * (p2[0] - p3[0]) + (p2[1] - p3[1]) * (p2[1] - p3[1]) + (p2[2] - p3[2]) * (p2[2] - p3[2]));
	//printf("LengthU: %f \nLengthV: %f\n", lengthU, lengthV);
}

void Patch::draw()
{

	Data * mydata = (((YAFScene*) scene)->getData());
	pair<float,float> coord = mydata->getCurrentTextureCoord();

	/*
	GLfloat textpoints[4][2] = {{ 0.0, 0.0},
	{ 0.0, 1.0}, 
	{ 1.0, 0.0},
	{ 1.0, 1.0} };*/

	GLfloat textpoints[4][2] = {{ 0.0, 0.0},
	{ 0.0, lengthV / coord.second}, 
	{ lengthU/ coord.first, 0.0},
	{ lengthU / coord.first, lengthV / coord.second} };

	GLint frontface;
	glGetIntegerv(GL_FRONT_FACE, &frontface);

	glFrontFace(GL_CW);
	glEnable(GL_AUTO_NORMAL);

	
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order+1,  0.0, 1.0, 3*(order+1), order+1,  &ctrlpoints[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints[0][0]); 



	glEnable(GL_MAP2_VERTEX_3);

	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMapGrid2f(partsU, 0.0,1.0, partsV, 0.0,1.0); 

	if(compute == "fill")
		glEvalMesh2(GL_FILL, 0,partsU, 0,partsV); 
	else if (compute == "point") 
		glEvalMesh2(GL_POINT, 0,partsU, 0,partsV); 
	else if (compute == "line") 
		glEvalMesh2(GL_LINE, 0,partsU, 0,partsV);

	glFrontFace(frontface);
}