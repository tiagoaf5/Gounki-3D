#include "YAFinterface.h"
#include <sstream>


#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

YAFinterface::YAFinterface()
{
	testVar=0;
}

void YAFinterface::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);
}

void YAFinterface::performPicking(int x, int y) 
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 0.1, 0.1, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer

	//data->drawSceneSelect();
	((YAFScene*) scene)->displaySelect();

	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void YAFinterface::processHits (GLint hits, GLuint buffer[]) 
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}
	
	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		for (int i=0; i<nselected; i++)
			printf("%d ",selected[i]);
		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}


void YAFinterface::initGUI()
{

	int i = 0, j = 0;
	char buf1[255];
	char buf2[255];

	data = (((YAFScene*) scene)->getData());

	//------------------------------------- LIGHTS ------------------------------

	GLUI_Panel *varPanel= addPanel("Lights", 1);
	vector <GLUI_Checkbox * > checkboxes;
	this->addColumn();

	size = (data->getLights()).size();

	while (i < (data->getLights()).size()){
		strcpy(buf1,(data->getLights())[i]->getID().c_str());
		checkboxes.push_back(addCheckboxToPanel(varPanel,buf1,&li[i],1));
		if (!data->getLights()[i]->getEnabled()){
			checkboxes[i]->set_int_val(0);
		}
		i++;
	}





	//------------------------------------------ DRAWMODE ----------------------------------

	GLUI_Panel *varPanel2= addPanel("DrawMode", 1);
	this->addColumn();
	GLUI_RadioGroup * radio_group = addRadioGroupToPanel(varPanel2, &grupo,2);


	b1 = addRadioButtonToGroup(radio_group,"Fill");
	b2 = addRadioButtonToGroup(radio_group,"Line");
	b3 = addRadioButtonToGroup(radio_group,"Point");

	if (data->getDrawMode() == "fill" || data->getDrawMode() == "Fill" ){
		radio_group->set_selected(0);
	}

	if (data->getDrawMode() == "point" || data->getDrawMode() == "Point" ){
		radio_group->set_selected(1);
	}

	if (data->getDrawMode() == "line" || data->getDrawMode() == "Line" ){
		radio_group->set_selected(2);
	}



	//----------------------------------------- CAMERAS -------------------------------------



	GLUI_Panel *varPanel3= addPanel("Cameras", 1);

	list_group = addListboxToPanel(varPanel3,"Cameras",&grupo2,3);

	cam = ((YAFScene*) scene)->getCamerasMap();

	map<string,CGFcamera*>::iterator iter = cam.begin();

	for (iter;iter != cam.end(); iter++){
		strcpy(buf2,(iter->first).c_str());
		list_group->add_item(j,buf2);

		if(iter->second == ((YAFScene*) scene)->getActiveCamera()){
			//(iter->second)->applyView();
			((YAFScene*) scene)->setActiveCamera(iter->first);
			list_group->set_int_val(j);
		}
		j++;
	}
	pos_default= j;
	list_group->add_item(j,"default");
}


void YAFinterface::processGUI(GLUI_Control *ctrl)
{
	switch (ctrl->user_id)
	{
	case 1:
		{
			updateLights();
			break;
		};

	case 2:
		{
			updateDrawMode();
			break;
		};

	case 3:
		{
			updateCameras();
			break;
		}
	}
}


void YAFinterface::updateLights(){

	int k = 0;

	while (k < size){
		if (li[k])
			(data->getLights())[k]->enable();
		else
			(data->getLights())[k]->disable();
		k++;
	}
}

void YAFinterface::updateDrawMode(){

	if (b1->active)
		data->setDrawMode("fill");
	else if (b2->active)
		data->setDrawMode("line");
	else
		data->setDrawMode("point");
}

void YAFinterface::updateCameras(){

	int k = 0;

	if(list_group->get_int_val() == pos_default){
		((YAFScene*) scene)->setActiveCamera("");
	}

	map<string,CGFcamera*>::iterator iter = cam.begin();

	for (iter;iter != cam.end(); iter++){
		if(list_group->get_int_val() == k){
			((YAFScene*) scene)->setActiveCamera(iter->first);
		}
		k++;
	}

}