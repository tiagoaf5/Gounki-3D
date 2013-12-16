#ifndef YAFinterface_H
#define YAFinterface_H

#include "CGFinterface.h"
#include "YAFscene.h"

class YAFinterface: public CGFinterface 
{
private:
	int light1,light2,light3,light4,light5;
	int testVar;
	int grupo,grupo2;
	GLUI_RadioButton *b1;
	GLUI_RadioButton *b2;
	GLUI_RadioButton *b3;
	GLUI_Listbox * list_group;
	int li[7];
	int size;
	int pos_default;

	Data *data;
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
};


#endif