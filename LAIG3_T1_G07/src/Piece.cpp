#include "Piece.h"
#include <sstream> 
#include <algorithm>
#include <map>

#include "CGFapplication.h"

using namespace std;

Piece::Piece(int player, PieceAppearances * thePiecesApp)
{
	this->player = player;
	this->thePiecesApp = thePiecesApp;
	animated = false;
	selected = false;
}

Piece::Piece(int player, PieceBase * piece, PieceAppearances * thePiecesApp)
{
	this->player = player;
	this->thePiecesApp = thePiecesApp;
	pieces.push_back(piece);
	animated = false;
	selected = false;
}

void Piece::draw()
{
	if(!selected)
	{
		thePiecesApp->apply(player);
	}
	else
		thePiecesApp->apply(3);


	for (int i = 0; i < pieces.size(); i++)
	{
		if(!animated)
		{
			glPushMatrix();
			glTranslatef(0.025*i,0.025*i,0);
			pieces[i]->draw();
			glPopMatrix();
		}
		else
		{
			selected = false;
			//printf(".");
			glPopMatrix();
			glPushMatrix();
			anim->applyTransforms();
			glTranslatef(0.025*i,0.025*i,0);
			pieces[i]->draw();
			glPopMatrix();
			glPushMatrix();
		}
	}
}

string Piece::getFormattedPiece() 
{		
	stringstream ss;

	ss << pieces[0]->getFormattedPiece();
	for (int i = 1; i < pieces.size(); i++)
		ss << "-" << pieces[i]->getFormattedPiece();

	ss << "-" << player;

	return ss.str();
}

void Piece::addPieces(vector<PieceBase *> p)
{
	for(int i = 0; i < p.size(); i++)
		pieces.push_back(p[i]);

	vector<string> ids;
	map<string, PieceBase *> pecas;

	for (int i = 0; i < pieces.size(); i++) 
	{
		stringstream ss;
		ss << pieces[i]->getFormattedPiece() << i;
		ids.push_back(ss.str());
		pecas.insert(pair<string, PieceBase *>(ss.str(),pieces[i])); 
	}

	sort(ids.begin(), ids.end());
	reverse(ids.begin(), ids.end());

	for (int i = 0; i < ids.size(); i++) 
		pieces[i] = pecas.find(ids[i])->second;
}

vector<PieceBase *> Piece::getPieces()
{
	return pieces;
}

int Piece::getPlayer() const
{
	return player;
}

void Piece::setAnimation(LinearAnimation * anim)
{
	if(anim != NULL) 
	{
		this->anim = anim; 
		animated = true;
	}
	else
	{
		this->anim = NULL;
		animated = false;
	}

}

void Piece::select()
{
	selected = true;
}

void Piece::unselect()
{
	selected = false;
}

void Piece::getNrPieces(int &c1,int &s1, int &c2, int &s2)
{
	string eaten = getFormattedPiece();
	string temp = eaten;

	if (!eaten.empty())
	{
		if (eaten.find("2") != -1)
		{
			while(temp.find("x") != -1)
			{
				s1++;
				temp = temp.substr(temp.find("x")+1);
			}
			while(temp.find("o") != -1)
			{
				c1++;
				temp = temp.substr(temp.find("o")+1);
			}
		}
		else
		{
			while(temp.find("x") != -1)
			{
				s2++;
				temp = temp.substr(temp.find("x")+1);
			}
			while(temp.find("o") != -1)
			{
				c2++;
				temp = temp.substr(temp.find("o")+1);
			}
		}
	}
}
