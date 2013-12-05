#include "SpotLight.h"



SpotLight::SpotLight(unsigned int id,bool enabled,float location[4],float ambient[4],float diffuse[4],float specular[4],float angle,float exponent,float direction[3]):
MyLight(id,location,direction){
	this->enabled = enabled;
	this->setAmbient(ambient);
	this->setDiffuse(diffuse);
	this->setSpecular(specular);
	this->setAngle(angle);
	this->exponent = exponent;
}

void SpotLight::applyLight(){

	if (enabled){
		glLightf(id, GL_SPOT_EXPONENT, exponent);
		CGFlight::update();
		//disabling alpha channel in order to lights be drawn properly
		glDisable (GL_BLEND);
		CGFlight::draw();
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		CGFlight::enable();
	}
	else{
		CGFlight::update();
		CGFlight::draw();
		CGFlight::disable();
	}
}

