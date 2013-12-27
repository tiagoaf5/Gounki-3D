#ifndef _TEXT_OBJECT_H_
#define _TEXT_OBJECT_H_

#include <stdio.h>

class TextObject
{
	public:
		TextObject(int type);
		void draw();
		void setNumber(char number);
	private:
		int type;
		char number;
		float posX,posY,posZ;
};

#endif
