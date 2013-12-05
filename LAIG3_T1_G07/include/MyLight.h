#ifndef _MYLIGHT_H_
#define _MYLIGHT_H_

#include <CGFlight.h>



class MyLight: public CGFlight
{
public:
	virtual void applyLight(void) = 0;
	MyLight(unsigned int id,float location[4],float direction[3]);
	void setIDS(string ids);
	string getID();
	bool getEnabled();
protected:
	string ids;
};



#endif
