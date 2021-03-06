#include "YAFinterface.h"

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
		data->getGame()->play(selected[0],selected[1]);
	}
	else
		printf("Nothing selected while picking \n");	
}


void YAFinterface::initGUI()
{
	dif1 = 1;
	dif2 = 1;

	//-------------------------------- GAME ------------------------------------

	GLUI_Panel *gamePanel= addPanel("Game", 1);
	gamePanel->set_alignment(GLUI_ALIGN_CENTER);
	this->addColumn();
	GLUI_Panel *scenePanel= addPanel("Scene", 1);
	gamePanel->set_alignment(GLUI_ALIGN_CENTER);

	//------------------------------------------------------------------------

	int i = 0, j = 0;
	char buf1[255];
	char buf2[255];

	data = (((YAFScene*) scene)->getData());
	game = data->getGame();



	//------------------------------------- LIGHTS ------------------------------
	GLUI_Panel *sceneopsPanel = addPanelToPanel(scenePanel,"",1);

	GLUI_Panel *lightsPanel= addPanelToPanel(sceneopsPanel,"Lights", 1);
	vector <GLUI_Checkbox * > checkboxes;
	addColumnToPanel(sceneopsPanel);

	size = (data->getLights()).size();

	while (i < (data->getLights()).size()){
		strcpy(buf1,(data->getLights())[i]->getID().c_str());
		checkboxes.push_back(addCheckboxToPanel(lightsPanel,buf1,&li[i],1));
		if (!data->getLights()[i]->getEnabled()){
			checkboxes[i]->set_int_val(0);
		}
		i++;
	}


	//------------------------------------------ DRAWMODE ----------------------------------

	GLUI_Panel *drawmodePanel= addPanelToPanel(sceneopsPanel,"DrawMode", 1);
	GLUI_RadioGroup * radio_group = addRadioGroupToPanel(drawmodePanel, &grupo,2);


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


	//------------------------------- Game Mode -------------------------------------

	GLUI_Panel *modePanel = addPanelToPanel(gamePanel,"Mode", 1);


	list_group2 = addListboxToPanel(modePanel,"",&grupo6,6);

	list_group2->add_item(1,"Player vs Player");
	list_group2->add_item(2,"Player vs Computer");
	list_group2->add_item(3,"Computer vs Player");
	list_group2->add_item(4,"Computer vs Computer");

	//------------------------------- Game Dificulty ----------------------------------

	GLUI_Panel *difficultyPanel = addPanelToPanel(gamePanel,"Difficulty", 1);

	GLUI_Panel *difficultyPanel1 = addPanelToPanel(difficultyPanel,"Computer 1", 1);
	addColumnToPanel(difficultyPanel);
	GLUI_Panel *difficultyPanel2 = addPanelToPanel(difficultyPanel,"Computer 2", 1);

	GLUI_RadioGroup * radio_group5 = addRadioGroupToPanel(difficultyPanel1, &grupo5,5);

	GLUI_RadioGroup * radio_group6 = addRadioGroupToPanel(difficultyPanel2, &grupo7,4);

	mode1 = addRadioButtonToGroup(radio_group5,"Easy");
	mode2 = addRadioButtonToGroup(radio_group5,"Hard");

	mode3 = addRadioButtonToGroup(radio_group6,"Easy");
	mode4 = addRadioButtonToGroup(radio_group6,"Hard");
	//mode3 = addRadioButtonToGroup(radio_group5,"Hard");

	radio_group5->set_selected(0);
	radio_group6->set_selected(0);

	//------------------------------ Server ------------------------------------



	addColumnToPanel(gamePanel);

	GLUI_Panel *servidorPanel= addPanelToPanel(gamePanel,"", 1);
	servidorPanel->set_alignment(GLUI_ALIGN_CENTER);

	b5 = addButtonToPanel(servidorPanel,"StartGame",7);
	b5->set_alignment(GLUI_ALIGN_CENTER);
	b6 = addButtonToPanel(servidorPanel,"Undo",8);
	b7 = addButtonToPanel(servidorPanel,"PlayMovie",9);

	//----------------------------------------- CAMERAS -------------------------------------



	GLUI_Panel *camerasPanel= addPanelToPanel(gamePanel,"Cameras", 1);

	list_group = addListboxToPanel(camerasPanel,"Cameras",&grupo2,3);

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
	list_group->add_item(j,"Default");
	list_group->add_item(j+1,"Mobile");

	list_group->set_int_val(j+1);
	((YAFScene*) scene)->setActiveCamera("MOBILECAMERA");


	//-------------------------------  Scenes --------------------------------------------

	GLUI_Panel *varPanel7 = addPanelToPanel(scenePanel,"Scenes", 1);

	list_group3 = addListboxToPanel(varPanel7,"Scenes",&grupo8,10);
	list_group3->add_item(1,"Garden");
	list_group3->add_item(2,"Prision");
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
		};
	case 4:
		{
			updateDifficulty(2);
			break;
		}
	case 5:
		{
			updateDifficulty(1);
			break;
		}
	case 6:
		{
			updateMode();
			break;
		}
	case 7:
		{
			game->startGame();
			break;
		}
	case 8:
		{
			game->pop();
			break;
		}
	case 9:
		{
			if(list_group->get_int_val() != 3 && list_group->get_int_val() != 2)
			{
				list_group->set_int_val(2);
				((YAFScene*) scene)->setActiveCamera("Player1");
			}
			game->playMovie();
			break;
		}
	case 10:
		{	
			updateScene();
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

	if(list_group->get_int_val() == pos_default)
		((YAFScene*) scene)->setActiveCamera("");
	else if(list_group->get_int_val() == pos_default+1)
		((YAFScene*) scene)->setActiveCamera("MOBILECAMERA");

	map<string,CGFcamera*>::iterator iter = cam.begin();

	for (iter;iter != cam.end(); iter++){
		if(list_group->get_int_val() == k){
			((YAFScene*) scene)->setActiveCamera(iter->first);
		}
		k++;
	}

}

void YAFinterface::updateDifficulty(int i)
{

	if(i == 1)
	{
		if (mode1->active)
			dif1 = 1;
		else if(mode2->active)
			dif1 = 2;
	}
	else
	{
		if(mode3->active)
			dif2 = 1;
		else if (mode4->active)
			dif2 = 2;
	}

	game->setDificulty(dif1,dif2);
}

void YAFinterface::updateMode(){

	if(list_group2->get_int_val() == 1){
		game->setMode(1);
		list_group->set_int_val(pos_default+1);
		((YAFScene*) scene)->setActiveCamera("MOBILECAMERA");
	}
	else if (list_group2->get_int_val() == 2){
		game->setMode(2);
		list_group->set_int_val(2);
		((YAFScene*) scene)->setActiveCamera("Player1");
	}
	else if (list_group2->get_int_val() == 3){
		game->setMode(3);
		list_group->set_int_val(3);
		((YAFScene*) scene)->setActiveCamera("Player2");
	}
	else {
		game->setMode(4);
		list_group->set_int_val(2);
		((YAFScene*) scene)->setActiveCamera("Player1");
	}

}

void YAFinterface::updateScene(){

	if(list_group3->get_int_val() == 1)
		data->setRoot("cena");
	else if(list_group3->get_int_val() == 2)
		data->setRoot("cena2");
}