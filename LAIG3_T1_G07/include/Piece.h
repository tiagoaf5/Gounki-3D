#ifndef _PIECE_H_
#define _PIECE_H_

#include <vector>

class Piece{
	vector<int> nrPe�as;
public:
	virtual string getFormattedPiece(); // obtem a pe�a e transforma em string
	virtual void draw();

};

#endif
