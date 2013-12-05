#include "Node.h"

#include <iostream> //remove

Node::Node(string id): id(id)
{
	/*glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, &transforms[0][0]);*/
	displaylist = false;
}

void Node::addChildStr(string child)
{
	childrenStr.push_back(child);
}

void Node::addprimitive(CGFobject * prim)
{
	primitives.push_back(prim);
}

void Node::addTransforms(float transforms1[4][4])
{
	for (int i = 0 ; i < 4; i++)
		for (int j = 0; j < 4; j++)
			transforms[i][j] = transforms1[i][j];
}

vector<string> Node::getChildrenStr() const
{
	return childrenStr;
}

void Node::addChild(Node * child)
{
	children.push_back(child);
}

void Node::printTransform() const
{
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j< 4; j++)
			printf("%f ", transforms[j][i]);
		printf("\n");
	}
}


void Node::applyTransforms()
{
	if(!animationStr.empty())
	{
		animation->applyTransforms();
	}
	
	glMultMatrixf(&transforms[0][0]);
	
}

void Node::drawPrimitives()
{
	for(unsigned int i = 0; i< primitives.size(); i++)
		primitives[i]->draw();
}

void Node::setAppearanceStr(string app)
{
	appearanceStr = app;
}

void Node::setAppearance(MyAppearance *app)
{
	appearance = app;
}

void Node::applyAppearance()
{
	if (!appearanceStr.empty())
	{
		//printf("Appearance %s:  \n", id);
		//	MyAppearance *abc = appearance;
		//string abc2 = appearance->getId();
		//cout << "**-appearance " << id << " - " << abc2 << endl;
		appearance->applyMyAppearance();
	}

}

MyAppearance * Node::getAppearance() const
{
	if (!appearanceStr.empty())
	{
		return appearance;
	}

	return NULL;
}

string Node::getAppearanceStr() const
{
	return appearanceStr;
}


bool Node::hasDisplaylist() const
{
	return displaylist;
}

void Node::setDisplaylist()
{
	displaylist = true;
}

void Node::setDisplaylistNum(int num)
{
	listnumber = num;
}


void Node::drawDisplayList()
{
	if(displaylist)
		glCallList(listnumber);
}

void Node::setAnimation(Animation * a)
{
	animation = a;
}

void Node::setAnimationStr(string ani)
{
	animationStr = ani;
}

string Node::getAnimationStr() const
{
	return animationStr;
}

