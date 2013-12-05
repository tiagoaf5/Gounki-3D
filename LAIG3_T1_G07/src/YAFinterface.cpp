#include "YAFinterface.h"
#include <sstream>

YAFinterface::YAFinterface()
{
	testVar=0;
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