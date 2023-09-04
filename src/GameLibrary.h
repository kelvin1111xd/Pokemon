/***********************************************************************
 * File: GameLibrary.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for GameLibrary class header
***********************************************************************/
#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "Enums.h"
using namespace std;

class Pokemon;
class Move;

class GameLibrary
{
private:
	map<string, Pokemon> pokemons;
	map<string, Move> moves;

public:
	GameLibrary() {};
	GameLibrary(const string& pokemonFileName, const string& moveFileName);

	const Pokemon& getPokemon(const string& name) const;
	const Move& getMove(const string& name) const;

	int changeToTypeId(std::string typeString);

	std::string changeToTypeName(int typeNum);

	std::string changeToEffectName(int effectNum);
};