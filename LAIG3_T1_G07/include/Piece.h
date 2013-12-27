#ifndef _PIECE_H_
#define _PIECE_H_

#include "PieceBase.h"
#include "CGFappearance.h"
#include "LinearAnimation.h"

#include <string>
#include <vector>

using namespace std;

class Piece
{
	CGFappearance * appearance;
	CGFappearance * selectedAppearance;
	int player;
	bool selected;
	vector<PieceBase *> pieces;
	bool animated;
	LinearAnimation * anim;
	int hidden;
public:
	Piece(int player, CGFappearance * appearance, CGFappearance * selectedAppearanca);
	Piece(int player, PieceBase * piece, CGFappearance * appearance, CGFappearance * selectedAppearanca);
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
};


#endif