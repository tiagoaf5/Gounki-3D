//#pragma once

#ifndef _DATA_H_
#define _DATA_H_


#include <map>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
//#include <sstream>

#include "cgfapplication.h"

#include "Node.h"
#include "MyAppearance.h"
#include "MyPerspectiveCamera.h"
#include "MyOrthogonalCamera.h"
#include "MyLight.h"
#include "MyShader.h"
#include "TextObject.h"
#include "Game.h"

/*
#include "Socket.h"
#include "Board.h"
*/

using namespace std;

class Data
{
private:
	//Graph's root
	string rootStr;
	Node * root;

	//default appearance
	MyAppearance * defaultAppearance;

	//Global properties
	float globalBackground[4];
	string globalDrawmode;
	string globalShading;
	string globalCullface;
	string globalCullorder;

	//Lighting properties
	bool lightingDoublesided;
	bool lightingLocal;
	bool lightingEnabled;
	float lightingAmbient[4];

	//data
	map<string, Node *> nodesMap;
	map<string, string> texturesMap;
	map<string, MyAppearance *> appearancesMap;
	vector<MyLight *> lights;

	//current texture coordinates
	pair<float,float> currentTextureCoord;

	//Appearances' stack
	stack<MyAppearance *> appearancesStack;

	//display lists
	stack<Node *> displaylistStack;
	vector<Node *> bfs;

	//animations
	map<string, Animation *>  animationsMap;
	vector<Animation *> animations;

	//shader
	vector<MyShader *> shaders;

	//picking
	CGFobject* pickingRetangle;

	//Sockets
	//Socket * socket;

	//Watch
	CGFobject* relogio;

	//Game
	Game * game;

	//Board
	//Board * board;

	//text
	TextObject *myTextObj1;
	TextObject *myTextObj2;


	//private functions
	void computeNodePointers(Node * node);
	void drawNode(Node * node);
	void clearAppearancesStack();
	void drawDisplayList(Node * node);
	void drawDisplayList(Node * node, int time);

public:
	Data();
	~Data();
	//graph
	bool addNodeToMap(char * id, Node * node);
	void printNodesMap(); //testing purpose
	void setRootStr(string r);
	void computeNodePointers();
	void drawScene();
	void drawSceneSelect();

	//texture
	void addTextureToMap(string id, string location);
	void printTexturesMap(); //testing purpose
	pair <float,float> getCurrentTextureCoord() const;
	void setCurrentTextureCoord(float x, float y);

	//appearances
	void addAppearanceToMap(string id, MyAppearance * appearance);
	void computeAppearancePointers();
	void printAppearancesMap(); //testing purpose

	//lights
	void setLighting(bool doublesided, bool local, bool enabled, float ambient[4]);
	void applyLighting();
	void addLight(MyLight *light);
	bool enableLight(unsigned int index);
	bool disableLight(unsigned int index);
	void applyLights();
	vector<MyLight*> getLights();

	//globals
	void setGlobals(float background[4], string drawmode, string shading, string cullface, string cullorder);
	void applyGlobals();
	void setDrawMode(string drawmode);
	string getDrawMode();

	//displayLists
	void computeDisplayLists();
	void doBfs();
	bool isMemberBFS (Node * node);

	//animations
	void addAnimationToMap(string id, Animation * a);
	vector<Animation *> getAnimations() const;

	//shaders
	void setShader (MyShader * sh);
	vector<MyShader *> getShaders();

	//picking
	void initPicking();
	/*
	//socket
	void initSocket();
	Socket * getSocket() const;*/

	//Watch
	void setClock(CGFobject* relogio);
	CGFobject* getClock();

	//Game
	void initGame();
	Game * getGame();

	//text
	TextObject * getText(int player);
	void setText(int player,int number);

	/*
	//board
	void initBoard();
	Board * getBoard();*/
};

#endif