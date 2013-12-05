#include "OmniLight.h"
#include "MyLight.h"


Omnilight::Omnilight(unsigned int id,bool enabled,float location[4],float ambient[4],float diffuse[4],float specular[4]):
MyLight(id,location,direction){
	this->enabled = enabled;
	this->setAmbient(ambient);
	this->setDiffuse(diffuse);
	this->setSpecular(specular);
	/*float __tmp_0[4] = {1,1,0,1};

	material = new CGFappearance(__tmp_0);*/
}

void Omnilight::applyLight(){

	if (enabled){
		//disabling alpha channel in order to lights be drawn properly
		glDisable (GL_BLEND);
		CGFlight::draw();
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		CGFlight::enable();
	}
	else{
		CGFlight::draw();
		CGFlight::disable();
	}
}
