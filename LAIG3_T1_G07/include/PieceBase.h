#ifndef _PIECEBASE_H_
#define _PIECEBASE_H_

#include <string>

class PieceBase
{
public:
	virtual std::string getFormattedPiece() = 0;
	virtual void draw() = 0;
};

#endif
