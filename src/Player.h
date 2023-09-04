/***********************************************************************
 * File: Player.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for Player class header
***********************************************************************/
#pragma once
#include <string>
#include <vector>
#include "Enums.h"
using namespace std;

class GameLibrary;
class Pokemon;

class Player
{
private:
	vector<Pokemon*> pokemons;
	int currentPokemon = 0;

public:
	Player() {}
	~Player()
	{
		for (auto& i : pokemons) delete(i);
	}

	void addPokemon(const GameLibrary& gLib, const string& name, const vector<string>& moves);

	//getter and setter...........................................................
	vector<Pokemon*>& getPokemons() { return pokemons; }

	const int& getCurrentPokemon() { return currentPokemon; }
	void setCurrentPokemon(const int& currentPokemon) { this->currentPokemon = currentPokemon; }
};

