#ifndef _PIECE_H_
#define _PIECE_H_

#include "PieceBase.h"
#include "CGFappearance.h"
#include "LinearAnimation.h"
#include "PiecesApperances.h"

#include <string>
#include <vector>

using namespace std;

class Piece
{
	PieceAppearances * thePiecesApp;
	int player;
	bool selected;
	vector<PieceBase *> pieces;
	bool animated;
	LinearAnimation * anim;
	int hidden;
public:
	Piece(int player, PieceAppearances * thePiecesApp);
	Piece(int player, PieceBase * piece, PieceAppearances * thePiecesApp);
	string getFormattedPiece(); // piece ready for prolog
	void draw();
	void addPieces(vector<PieceBase *> p);
	vector<PieceBase *> getPieces();
	int getPlayer() const;
	void setAnimation(LinearAnimation * anim);
	void setHidden(int number);
	int getHidden();
	void select();
	void unselect();
	void getNrPieces(int &c1,int &s1, int &c2, int &s2);
};


#endif