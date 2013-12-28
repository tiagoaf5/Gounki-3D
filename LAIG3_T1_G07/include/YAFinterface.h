#ifndef YAFinterface_H
#define YAFinterface_H

#include "CGFinterface.h"
#include "YAFscene.h"

class YAFinterface: public CGFinterface 
{
private:
	int light1,light2,light3,light4,light5;
	int testVar;
	int grupo,grupo2,grupo3,grupo5,grupo6, grupo7;
	GLUI_RadioButton *b1;
	GLUI_RadioButton *b2;
	GLUI_RadioButton *b3;
	GLUI_RadioButton *mode1,*mode2,*mode3,*mode4;
	GLUI_Listbox * list_group, *list_group2;
	GLUI_Button *b4;
	GLUI_Button *b5;
	GLUI_Button *b6;
	GLUI_Button *b7;
	int li[7];
	int size;
	int pos_default;

	Data *data;
	Game *game;
	map<string,CGFcamera*> cam;

public:
	YAFinterface();

	virtual void initGUI();
	virtual void processGUI(GLUI_Control *ctrl);
	void updateLights();
	void updateDrawMode();
	void updateCameras();
	virtual void processMouse(int button, int state, int x, int y);	
	void performPicking(int x, int y);
	void processHits(GLint hits, GLuint buffer[]);
	void updateDifficulty();
	void updateMode();
};


#endif