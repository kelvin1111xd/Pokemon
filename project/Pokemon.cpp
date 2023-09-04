/***********************************************************************
 * File: Pokemon.cpp
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for Pokemon class cpp
***********************************************************************/
#include "Pokemon.h"
#include "GameLibrary.h"
#include "Move.h"

// Intent:	Pokemon copy constructor
// Pre:		other Pokemon
// Post:	return a copy of pokemon
Pokemon::Pokemon(const Pokemon& other)
{
	// copy data
	name = other.name;
	type = other.type;
	currentHp = other.currentHp;
	maxHp = other.maxHp;
	atk = other.atk;
	def = other.def;
	spAtk = other.spAtk;
	spDef = other.spDef;
	speed = other.speed;
	status = other.status;

	// copy move
	for (auto& i : other.moves)
	{
		Move* temp = new Move(*i);
		moves.push_back(temp);
	}
}

// Intent:	add move to pokemon
// Pre:		the name of move and it's library
// Post:	None
void Pokemon::addMove(const GameLibrary& gLib, string name)
{
	// copy move
	Move* temp = new Move(gLib.getMove(name));

	// add move
	moves.push_back(temp);
}