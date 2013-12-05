#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <string>
#include "cgfobject.h"
#include "MyAppearance.h"
#include "Animation.h"

using namespace std;

class Node
{
private:
	string id;
	//primitives
	vector<CGFobject *> primitives;
	//transforms
	float transforms[4][4];
	//children
	vector<string> childrenStr;
	vector<Node *> children;
	//appearance
	string appearanceStr;
	MyAppearance * appearance;
	//display list
	bool displaylist;
	int listnumber;
	//animation
	Animation * animation;
	string animationStr;

public:
	Node(string id);
	void addChildStr(string child);
	void addprimitive(CGFobject * prim);
	string getId() const {return id;};
	void addTransforms(float transforms1[4][4]);
	void addChild(Node * child);
	vector<string> getChildrenStr() const;
	int getNoChildren() const {return children.size();};
	void printTransform() const;
	vector<Node *> getChildren() const {return children;};
	void applyTransforms();
	void drawPrimitives();
	//appearances
	void setAppearanceStr(string app);
	string getAppearanceStr() const;
	void setAppearance(MyAppearance *app);
	void applyAppearance();
	MyAppearance * getAppearance() const;
	//displaylist
	bool hasDisplaylist() const;
	void setDisplaylist();
	void setDisplaylistNum(int num);
	void drawDisplayList();
	//animation
	void setAnimationStr(string ani);
	string getAnimationStr() const;
	void setAnimation(Animation * a);


};

#endif