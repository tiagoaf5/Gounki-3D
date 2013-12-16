#ifndef _PIECE_H_
#define _PIECE_H_

#include <string>

class PieceBase
{
public:
	PieceBase();
	virtual std::string getFormattedPiece();
	virtual void draw();
};

#endif
