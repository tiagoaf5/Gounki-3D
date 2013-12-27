#include "Piece.h"
#include <sstream> 
#include <algorithm>
#include <map>

#include "CGFapplication.h"

//test purpose
#include <iostream>
using namespace std;

Piece::Piece(int player, CGFappearance * appearance, CGFappearance * selectedAppearanca)
{
	this->player = player;
	this->appearance = appearance;
	this->selectedAppearance = selectedAppearanca;
	animated = false;
	selected = false;
	hidden = 1;
}

Piece::Piece(int player, PieceBase * piece, CGFappearance * appearance, CGFappearance * selectedAppearanca)
{
	this->player = player;
	this->appearance = appearance;
	this->selectedAppearance = selectedAppearanca;
	pieces.push_back(piece);
	animated = false;
	selected = false;
	hidden = 1;
}

void Piece::draw()
{
	if(!selected)
		appearance->apply();
	else
		selectedAppearance->apply();

	if(hidden == 1){
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
	else{
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
	printf(";\n");
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
	{
		cout << ids[i] << endl;
		pieces[i] = pecas.find(ids[i])->second;
	}
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

void Piece::setHidden(int number){
	hidden = number;
}

int Piece::getHidden(){
	return hidden;
}

void Piece::select()
{
	selected = true;
}

void Piece::unselect()
{
	selected = false;
}
