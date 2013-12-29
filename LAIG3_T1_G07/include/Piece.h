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
	PieceAppearances * thePiecesApp; //appearances
	int player; //owner
	bool selected;
	vector<PieceBase *> pieces; //simplePieces
	//animation options
	bool animated; 
	LinearAnimation * anim;
	
public:
	Piece(int player, PieceAppearances * thePiecesApp);
	Piece(int player, PieceBase * piece, PieceAppearances * thePiecesApp);
	
	//piece ready for prolog
	string getFormattedPiece(); 

	//draw
	void draw();

	//Base pieces
	void addPieces(vector<PieceBase *> p);
	vector<PieceBase *> getPieces();

	//player
	int getPlayer() const;
	
	//animation
	void setAnimation(LinearAnimation * anim);
	
	//(un)select
	void select();
	void unselect();

	//get number of each pieces type
	void getNrPieces(int &c1,int &s1, int &c2, int &s2);
};


#endif