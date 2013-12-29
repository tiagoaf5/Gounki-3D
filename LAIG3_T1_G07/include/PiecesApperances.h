#ifndef _PIECEAPPEARANCES_H_
#define _PIECEAPPEARANCES_H_

#include "CGFappearance.h"

class PieceAppearances {
public:
	PieceAppearances(CGFappearance * w,CGFappearance * b,CGFappearance * s);
	void setAppearances(CGFappearance * w,CGFappearance * b);
	void apply(int player);
private:
	CGFappearance * white;
	CGFappearance * black;
	CGFappearance * selected;
};

#endif