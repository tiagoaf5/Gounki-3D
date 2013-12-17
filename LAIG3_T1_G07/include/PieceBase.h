#ifndef _PIECEBASE_H_
#define _PIECEBASE_H_

#include <string>

class PieceBase
{
public:
	PieceBase(){};
	virtual std::string getFormattedPiece(){return "a";};
	virtual void draw(){};
};

#endif
