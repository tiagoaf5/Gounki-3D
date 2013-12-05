#include "MyLight.h"


MyLight::MyLight(unsigned int id,float location[4],float direction[3]):
CGFlight(id,location,direction){
}

void MyLight::setIDS(string ids)
{
	this->ids = ids;
}

string MyLight::getID(){
	
	return ids;
}

bool MyLight::getEnabled(){

	return enabled;
}