#ifndef _FIXEDTEXT_H_
#define _FIXEDTEXT_H_

#include <string>
using std::string;

class FixedText
{
private:
	float color[3];
	string text;
	void * font;
	bool visible;
	//private functions
	void begin2D(bool withDepthTest);
	void end2D();
public:
	FixedText(string text, float color[3]);
	void draw();
	void setText(string t);
	void hide();
	void show();
};


#endif