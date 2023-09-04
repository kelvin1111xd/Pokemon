/***********************************************************************
 * File: Player.cpp
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for Player class cpp
***********************************************************************/
#include "Player.h"
#include "GameLibrary.h"
#include "Pokemon.h"
#include "Move.h"

// Intent:	add a pokemon with moves to player
// Pre:		pokemon name and it's moves name, with those data's library 
// Post:	None
void Player::addPokemon(const GameLibrary& gLib, const string& name, const vector<string>& moves)
{
	// a copy of pokemon
	Pokemon* temp = new Pokemon(gLib.getPokemon(name));

	// input to player
	pokemons.push_back(temp);

	// add moves to pokemon
	for(const auto& i : moves) pokemons.back()->addMove(gLib, i);
}