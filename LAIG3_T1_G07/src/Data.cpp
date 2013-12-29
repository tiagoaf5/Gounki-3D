#include "Data.h"
#include <queue>
#include "YAFScene.h"
#include "MyRectangle.h"

Data::Data()
{
	float amb[] = {0.1,0.1,0.1,1};
	float dif[] = {0.1,0.1,0.1,1};
	float sp[4] = {0.1,0.1,0.1,1};
	float em[4] = {0.1,0.1,0.1,1};
	float sh = 0;
	defaultAppearance = new MyAppearance(amb, dif, sp, em, sh);
	defaultAppearance->setId("default:default");
	appearancesStack.push(defaultAppearance);


	game = NULL;
}


Data::~Data()
{
	delete(defaultAppearance);

	map<string, Node *>::iterator it_nodes = nodesMap.begin();
	for(;it_nodes != nodesMap.end() ;it_nodes++)
		delete(it_nodes->second);

	map<string, MyAppearance *>::iterator it_app = appearancesMap.begin();
	for(;it_app != appearancesMap.end() ;it_app++)
		delete(it_app->second);

	for (unsigned int i = 0; i < lights.size(); i++)
		delete(lights[i]);
}

bool Data::addNodeToMap(char * id, Node * node)
{
	map<string, Node *>::iterator it;

	it = nodesMap.find(id);

	if(it != nodesMap.end() && node != NULL)
	{
		if(it->second != NULL)
		{
			cout << "Redefinition of node " << id << endl;
			exit(1);
		}
		it->second = node;
		return true;
	}
	else
	{
		nodesMap.insert(pair<char *, Node *>(id,node));
		return true;
	}

	return false;
}


void Data::setRootStr(string r)
{
	rootStr = r;
}

void Data::setRoot(string r)
{
	root = nodesMap.find(r)->second;
	if(r == "cena")
	{
		nodesMap.find("board")->second->setAppearance(appearancesMap.find("ap_board1")->second);
		nodesMap.find("board_lateral")->second->setAppearance(appearancesMap.find("ap_board_side1")->second);
		if(game != NULL)
			game->setAppearances(appearancesMap.find("ap_black_piece")->second, appearancesMap.find("ap_white_piece")->second);
	}
	else
	{
		nodesMap.find("board")->second->setAppearance(appearancesMap.find("ap_board")->second);
		nodesMap.find("board_lateral")->second->setAppearance(appearancesMap.find("ap_board_side")->second);

		if(game != NULL)
			game->setAppearances(appearancesMap.find("ap_black_piece1")->second, appearancesMap.find("ap_white_piece1")->second);
	}

}

void Data::computeNodePointers()
{
	root = nodesMap.find(rootStr)->second;

	if (root == NULL)
	{
		cout << "The root " << rootStr << " is not defined!" << endl;
		exit (1);
	}
	else
		computeNodePointers(root);
}


void Data::computeNodePointers(Node * node)
{
	if (node->getNoChildren() != 0)
		return;

	string app = node->getAppearanceStr();

	if (!app.empty())
	{
		map<string, MyAppearance *>::iterator it = appearancesMap.find(app);

		if (it == appearancesMap.end())
		{
			cout << "Node's " << node->getId() << " appearance " << app << " not defined" << endl;
			exit(1);
		}
		MyAppearance * abc =  it->second;
		//cout << "--------------> " << abc->getId() << " <----------"<<endl;
		node->setAppearance(abc);
	}

	string anim = node->getAnimationStr();

	if(!anim.empty())
	{
		map<string, Animation *>::iterator it = animationsMap.find(anim);

		if(it == animationsMap.end())
		{
			cout << "Node's " << node->getId() << " animation " << anim << " not defined" << endl;
			exit(1);
		}

		node->setAnimation(it->second);
	}

	//cout << "--> " << node->getId() << endl;

	vector<string> children = node->getChildrenStr();

	for (unsigned int i = 0; i < children.size(); i++)
	{
		Node * child = nodesMap.find(children[i])->second;
		if (child == NULL) 
		{
			cout << "The node " << children[i] << " is not defined!" << endl;
			exit(1);
		}
		else
		{
			node->addChild(child);
			computeNodePointers(child);
		}
	}
}


void Data::drawScene()
{
	//board->draw();
	game->draw();
	//glPushMatrix();
	/*for (int r=0; r < 8; r++)
	{
	glPushMatrix();
	for (int c=0; c < 8; c++)
	{
	glPushMatrix();
	glTranslatef(11.14375+0.1875*r,2.11,11.09375+0.1875*c);
	glRotatef(-90,1,0,0);
	obj->draw();
	glPopMatrix();
	}
	glPopMatrix();
	}*/
	//glPopMatrix();
	clearAppearancesStack();
	drawNode(root);
	game->drawText();
}

void Data::drawSceneSelect()
{
	glPushName(-1);

	//glPushMatrix();

	for (int r=0; r < 8; r++)
	{
		glPushMatrix();
		glLoadName(r);
		for (int c=0; c < 8; c++)
		{
			glPushMatrix();
			glTranslatef(11.14375+0.1875*r,2.11,11.09375+0.1875*c);
			glRotatef(-90,1,0,0);
			glPushName(c);
			pickingRetangle->draw();
			glPopName();
			glPopMatrix();
		}
		glPopMatrix();
	}
	//glPopMatrix();
}

void Data::drawDisplayList(Node * node)
{
	drawDisplayList(node,0);
}

void Data::drawDisplayList(Node * node, int time)
{
	if(node->hasDisplaylist() && time > 0)
	{
		if(node->getAppearance() == NULL)
		{
			MyAppearance * ap = appearancesStack.top();
			ap->applyMyAppearance();
			currentTextureCoord = ap->getTexLength();
		}
		node->drawDisplayList();
		return;
	}

	glPushMatrix();

	//apply node's transforms
	node->applyTransforms();

	//taking care of appearances
	MyAppearance * myapp = NULL;

	if (time == 0)
	{
		clearAppearancesStack();
		appearancesStack.pop();
		currentTextureCoord.first = 1;
		currentTextureCoord.second = 1;
	}

	time++;

	if(node->getAppearance() != NULL)
		myapp = node->getAppearance();
	else if (!appearancesStack.empty())
		myapp = appearancesStack.top();

	//applying appearance if exists
	if (myapp != NULL) {
		appearancesStack.push(myapp);
		myapp->applyMyAppearance();
		// no problem in losing data due to recursion because default coordinates are applyed 
		currentTextureCoord = myapp->getTexLength(); 
	}


	//draw primitives
	node->drawPrimitives();

	vector<Node *> children = node->getChildren();

	//draw children
	for (unsigned int i = 0; i < children.size(); i++)
	{
		drawDisplayList(children[i],time);
		if(myapp != NULL)
			currentTextureCoord = myapp->getTexLength();
	}

	if (!appearancesStack.empty())
		appearancesStack.pop();

	glPopMatrix();
}

void Data::drawNode(Node * node)
{
	if(node->hasDisplaylist())
	{
		if(node->getAppearance() == NULL)
		{
			MyAppearance * ap = appearancesStack.top();
			//cout << ":" << ap->getId();
			ap->applyMyAppearance();
			currentTextureCoord = ap->getTexLength();
		}
		node->drawDisplayList();
	}
	else
	{
		glPushMatrix();

		//apply node's transforms
		node->applyTransforms();


		//apply appearance
		MyAppearance * myapp;

		if (node->getAppearance() == NULL) 
			myapp = appearancesStack.top();
		else
			myapp = node->getAppearance();

		appearancesStack.push(myapp);

		myapp->applyMyAppearance();

		// no problem in losing data due to recursion because default coordinates are applyed 
		currentTextureCoord = myapp->getTexLength(); 


		//draw primitives
		node->drawPrimitives();

		vector<Node *> children = node->getChildren();

		//draw children
		for (unsigned int i = 0; i < children.size(); i++)
		{
			drawNode(children[i]);
			currentTextureCoord = myapp->getTexLength();
		}

		appearancesStack.pop();
		glPopMatrix();
	}
}


void Data::addTextureToMap(string id, string location)
{
	if  (texturesMap.find(id) != texturesMap.end())
	{
		cout << "Texture " << id << " already defined" << endl;
		exit(1);
	}
	texturesMap.insert(pair<string,string>(id,location));
}

void Data::addAppearanceToMap(string id, MyAppearance * appearance)
{
	if  (appearancesMap.find(id) != appearancesMap.end())
	{
		cout << "Appearance " << id << " already defined" << endl;
		exit(1);
	}
	appearancesMap.insert(pair<string, MyAppearance *>(id,appearance));
}


void Data::computeAppearancePointers()
{
	map<string, MyAppearance *>::iterator it = appearancesMap.begin();

	for(;it != appearancesMap.end(); it++)
	{
		string textureId = it->second->getTextureId();
		if (!textureId.empty()) // it has a texture
		{
			map<string, string>::iterator it2 = texturesMap.find(textureId);

			if (it2 == texturesMap.end())
			{
				cout << "Appearance " << it->second->getId() << " defined texture " << textureId << " which doesn't exist" << endl;
				exit(1);
			}
			else
			{
				it->second->setTexture(it2->second);
			}
		}
	}

}


void Data::addLight(MyLight* light){
	lights.push_back(light);
}

void Data::applyLights(){
	for (unsigned int i = 0; i < lights.size(); i++)
		lights[i]->applyLight();

}

void Data::setGlobals(float background[4], string drawmode, string shading, string cullface, string cullorder)
{
	globalBackground[0] = background[0];
	globalBackground[1] = background[1];
	globalBackground[2] = background[2];
	globalBackground[3] = background[3];

	if (!(drawmode == "line" || drawmode == "fill" || drawmode == "point"))
	{
		cout << "Invalid drawmode " << drawmode << endl;
		exit(1);
	}
	globalDrawmode = drawmode;

	if (!(shading == "flat" || shading == "gouraud"))
	{
		cout << "Invalid shading " << shading << endl;
		exit(1);
	}
	globalShading = shading;

	if (!(cullface == "none" || cullface == "front" || 
		cullface == "back" || cullface == "both"))
	{
		cout << "Invalid cullface " << cullface << endl;
		exit(1);
	}
	globalCullface = cullface;

	if (!(cullorder == "ccw" || cullorder == "cw"
		||cullorder == "CCW" || cullorder == "CW"))
	{
		cout << "Invalid cullorder " << cullorder << endl;
		exit(1);
	}
	globalCullorder = cullorder;
}

void Data::applyGlobals()
{
	glClearColor(globalBackground[0],globalBackground[1],globalBackground[2],globalBackground[3]);

	if(globalDrawmode == "fill")
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	else if(globalDrawmode == "line")
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	else if(globalDrawmode == "point")
		glPolygonMode( GL_FRONT_AND_BACK, GL_POINT);

	if(globalShading == "flat")
		glShadeModel(GL_FLAT);
	else if(globalShading == "gouraud")
		glShadeModel(GL_SMOOTH);

	if(globalCullface == "none")
		glDisable(GL_CULL_FACE);
	else
	{
		glEnable(GL_CULL_FACE);
		if (globalCullface == "back")
			glCullFace(GL_BACK);
		else if (globalCullface == "front")
			glCullFace(GL_FRONT);
		else if (globalCullface == "both")
			glCullFace(GL_FRONT_AND_BACK);
	}

	if(globalCullorder == "ccw" || globalCullorder == "CCW")
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
}

void Data::setDrawMode(string drawmode)
{
	globalDrawmode = drawmode;
}

void Data::setLighting(bool doublesided, bool local, bool enabled, float ambient[4])
{
	lightingDoublesided = doublesided;
	lightingLocal = local;
	lightingEnabled = enabled;
	lightingAmbient[0] = ambient[0];
	lightingAmbient[1] = ambient[1];
	lightingAmbient[2] = ambient[2];
	lightingAmbient[3] = ambient[3];
}

void Data::applyLighting()
{
	if (lightingDoublesided)
		glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	else
		glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	if (lightingLocal)
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	if (lightingEnabled)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightingAmbient);
}

pair <float,float> Data::getCurrentTextureCoord() const
{
	return currentTextureCoord;
}

void Data::setCurrentTextureCoord(float x, float y)
{
	currentTextureCoord.first = x;
	currentTextureCoord.second = y;
}



string Data::getDrawMode(){
	return globalDrawmode;
}

vector<MyLight*> Data::getLights(){

	return lights;
}

void Data::doBfs()
{
	queue<Node *> q;

	bfs.push_back(root);
	q.push(root);

	while(!q.empty())
	{
		Node * v = q.front();
		vector<Node *> w = v->getChildren();

		for(unsigned int i = 0; i < w.size(); i++)
		{
			if(!isMemberBFS(w[i]))
			{
				if(w[i]->hasDisplaylist())
					displaylistStack.push(w[i]);
				bfs.push_back(w[i]);
				q.push(w[i]);
			}
		}
		q.pop();
	}
}

bool Data::isMemberBFS (Node * node)
{
	for (unsigned int i = 0; i < bfs.size(); i++)
		if (bfs[i]->getId() == node->getId())
			return true;
	return false;
}


void Data::clearAppearancesStack()
{
	while (!appearancesStack.empty())
		appearancesStack.pop();
	appearancesStack.push(defaultAppearance);
}


void Data::computeDisplayLists()
{

	while (!displaylistStack.empty())
	{
		Node * mynode = displaylistStack.top();
		int dl=glGenLists(1);

		glNewList(dl,GL_COMPILE);
		drawDisplayList(mynode);
		glEndList();

		mynode->setDisplaylistNum(dl);
		displaylistStack.pop();
	}
}


void Data::addAnimationToMap(string id, Animation * a)
{
	if  (animationsMap.find(id) != animationsMap.end())
	{
		cout << "Animation " << id << " already defined" << endl;
		exit(1);
	}
	animationsMap.insert(pair<string, Animation *>(id,a));
	animations.push_back(a);
}

vector<Animation *> Data::getAnimations() const
{
	return animations;
}


void Data::setShader (MyShader * sh) 
{
	shaders.push_back(sh);
}
vector<MyShader *> Data::getShaders()
{
	return shaders;
};


void Data::initPicking()
{
	pickingRetangle = new MyRectangle(-0.091,-0.091,0.091,0.091,NULL);
}

void Data::setClock(CGFobject* relogio){
	this->relogio = relogio;
}

CGFobject* Data::getClock(){
	return relogio;
}

void Data::initGame()
{
	MyAppearance * app1 = appearancesMap.find("ap_white_piece")->second;
	MyAppearance * app2 = appearancesMap.find("ap_black_piece")->second;
	MyAppearance * app3 = appearancesMap.find("ap_selected_piece")->second;

	game = new Game(app2,app1,app3);
	game->setClock(relogio);

	//send game textures needed for score
	initGameScore();
}

void Data::initGameScore()
{

	vector<CGFappearance * > nums;
	for (int i = 0 ; i < 10; i++)
	{
		stringstream ss;
		ss << "ap_" << i;
		nums.push_back(appearancesMap.find(ss.str())->second);
	}

	game->setNumberAppearances(nums);
}


Game * Data::getGame()
{
	return game;
}

/*Functions for testing purpose*/

void Data::printTexturesMap() //testing purpose
{
	map<string, string>::iterator it = texturesMap.begin();

	cout << "___________________________________\n.....................................\n";

	printf("___________________________________\n.....................................\n");

	for(;it != texturesMap.end(); it++)
	{
		cout << it->first << ": " << it->second << endl;
	}
}


void Data::printAppearancesMap() //testing purpose
{
	map<string, MyAppearance *>::iterator it = appearancesMap.begin();

	cout << "___________________________________\n.....................................\n";

	printf("___________________________________\n.....................................\n");

	for(;it != appearancesMap.end(); it++)
	{
		cout << it->first << ": " <<  it->second->getId() << endl;
	}
}





void Data::printNodesMap()
{
	map<string, Node *>::iterator it = nodesMap.begin();

	cout << "___________________________________\n.....................................\n";
	printf("___________________________________\n.....................................\n");

	for(;it != nodesMap.end(); it++)
	{
		if (it->second != NULL)
		{
			cout << it->first << ": " << it->second->getId() << endl;
			//printf ("%s: %s\n",it->first, it->second->getId());
		}
		else
		{
			cout << it->first << ": NULL" << endl;
			//printf ("%s: NULL\n",it->first);
		}
	}
}


