#ifndef _PIECE_H_
#define _PIECE_H_

#include <vector>

class Piece{
	vector<int> nrPeças;
public:
	virtual string getFormattedPiece(); // obtem a peça e transforma em string
	virtual void draw();

};

#endif
