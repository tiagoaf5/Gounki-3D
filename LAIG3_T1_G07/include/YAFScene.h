#ifndef _YAFSCENE_H_
#define _YAFSCENE_H_

#include <map>
#include <string>

#include "tinyxml.h"
#include "CGFscene.h"
#include "cgfobject.h"

#include "Node.h"
#include "MyRectangle.h"
#include "Triangle.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Torus.h"
#include "Plane.h"
#include "Waterline.h"
#include "Patch.h"
#include "Vehicle.h"
#include "myClock.h"

#include "Data.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "MyOrthogonalCamera.h"
#include "MyPerspectiveCamera.h"
#include "LinearAnimation.h"

//#include "Socket.h"


using namespace std;


class YAFScene: public CGFscene
{
public:

	YAFScene(char *filename);
	~YAFScene();

	//Scene
	void init();				
	void display();
	void update(unsigned long t);

	//Data
	Data * getData();

	//Camera info
	void setActiveCamera(CGFcamera * cam);
	void addCamera(CGFcamera * cam, string camid);
	bool setActiveCamera(string camid);
	void applyCamera();
	CGFcamera * getActiveCamera() const;
	map<string, CGFcamera *> getCamerasMap() const;
	void displaySelect();

protected:
	TiXmlDocument* doc;
	TiXmlElement* globalsElement; 
	TiXmlElement* camerasElement;
	TiXmlElement* lightingElement;
	TiXmlElement* texturesElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* animationsElement;
	TiXmlElement* graphElement;

private:
	char * filename;
	Data * data;
	bool stringToBool (char * str, bool & value);
	bool stringToArray (char * str, int nParam, float values[]);
	bool readYaf(char *filename);
	bool readGlobalsBlock ();
	bool readCamerasBlock ();
	bool readLightingBlock ();
	bool readTexturesBlock ();
	bool readAppearancesBlock ();
	bool readGraphBlock ();
	bool readAnimationsBlock();

	

	map<string, CGFcamera *> camerasMap;
};


#endif
