#ifndef _PIECECIRCLE_H_
#define _PIECECIRCLE_H_



class Piece
{
	MyAppearance * appearance;
	int player;
	bool selected;
	vector<PieceBase *> pieces;
public:
	Piece(int player, MyAppearance * appearance);
	Piece(int player, PieceBase * piece, MyAppearance * appearance);
	std::string getFormattedPiece(); // obtem a peça e transforma em string
	void draw();
	void addPiece(PieceBase * piece);
};


#endif