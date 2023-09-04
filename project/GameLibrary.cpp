/***********************************************************************
 * File: GameLibrary.cpp
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for GameLibrary class cpp
***********************************************************************/
#include "GameLibrary.h"
#include "Pokemon.h"
#include "Move.h"
#include <iostream>

// Intent:	GameLibrary constructor
// Pre:		pokemons file name and moves file name
// Post:	create a library for pokemon and move
GameLibrary::GameLibrary(const string& pokemonFileName, const string& moveFileName)
{
	// create move library
    ifstream file(moveFileName);

    if (file.is_open())
    {
		// read file
        string data;

        while (getline(file, data))
        {
			// read and input move
            stringstream ss(data);
            vector<string> moveData(7, "");

            for (int i = 0; i < 7; i++) ss >> moveData[i];

            bool isSpecialAttack = moveData[2] == "Special";

            Move move(moveData[0], changeToTypeId(moveData[1]), stoi(moveData[3]), stoi(moveData[4]), stoi(moveData[5]), isSpecialAttack, changeToTypeId(moveData[6]));
            moves[moveData[0]] = move;
        }
    }
	
    file.close();

	// create pokemon library
    file.open(pokemonFileName);

    if (file.is_open())
    {
		// read file
        string name = "";

        while (file >> name)
        {
			// read and input pokemon
            int typeNum;
            file >> typeNum;
            vector<int> type(typeNum, -1);
            vector<int> pokemonData(6, 0);

            for (int i = 0; i < typeNum; i++)
            {
                string typeStr = "";
                file >> typeStr;
                type[i] = changeToTypeId(typeStr);
            }

            for (int i = 0; i < 6; i++) file >> pokemonData[i];

            Pokemon pokemon(name, type, pokemonData[0], pokemonData[1], pokemonData[2], pokemonData[3], pokemonData[4], pokemonData[5]);
            pokemons[name] = pokemon;
        }
    }
}

// Intent:	get pokemon in library
// Pre:		pokemon name
// Post:	return a copy of the pokemon
const Pokemon& GameLibrary::getPokemon(const string& name) const 
{
	return pokemons.at(name);
}


// Intent:	get move in library
// Pre:		move name
// Post:	return a copy of move
const Move& GameLibrary::getMove(const string& name) const 
{
	return moves.at(name);
}

// Intent:	change type in string to int
// Pre:		string of type
// Post:	return int of type
int GameLibrary::changeToTypeId(std::string typeString)
{
	if (typeString == "Normal") return TYPE::NORMAL;
	else if (typeString == "Fire") return TYPE::FIRE;
	else if (typeString == "Water") return TYPE::WATER;
	else if (typeString == "Electric") return TYPE::ELECTRIC;
	else if (typeString == "Grass") return TYPE::GRASS;
	else if (typeString == "Ice") return TYPE::ICE;
	else if (typeString == "Fighting") return TYPE::FIGHTING;
	else if (typeString == "Poison") return TYPE::POISON;
	else if (typeString == "Ground") return TYPE::GROUND;
	else if (typeString == "Flying") return TYPE::FLYING;
	else if (typeString == "Psychic") return TYPE::PSYCHIC;
	else if (typeString == "Bug") return TYPE::BUG;
	else if (typeString == "Rock") return TYPE::ROCK;
	else if (typeString == "Ghost") return TYPE::GHOST;
	else if (typeString == "Dragon") return TYPE::DRAGON;
	else if (typeString == "Dark") return TYPE::DARK;
	else if (typeString == "Steel") return TYPE::STEEL;
	else if (typeString == "Fairy") return TYPE::FAIRY;

	if (typeString == "") return EFFECT::NONE;
	else if (typeString == "PAR") return EFFECT::PAR;
	else if (typeString == "BRN") return EFFECT::BUR;
	else if (typeString == "PSN") return EFFECT::PSN;
}

// Intent:	change type in int to string
// Pre:		int of type
// Post:	return string of type
std::string GameLibrary::changeToTypeName(int typeNum)
{
	switch (typeNum)
	{
	case TYPE::NORMAL: return "Normal";
	case TYPE::FIRE: return "Fire";
	case TYPE::WATER: return "Water";
	case  TYPE::ELECTRIC: return "Electric";
	case  TYPE::GRASS: return "Grass";
	case TYPE::ICE: return "Ice";
	case  TYPE::FIGHTING: return "Fighting";
	case  TYPE::POISON: return "Poison";
	case  TYPE::GROUND: return "Ground";
	case  TYPE::FLYING: return "Flying";
	case  TYPE::PSYCHIC: return "Psychic";
	case  TYPE::BUG: return "Bug";
	case  TYPE::ROCK: return "Rock";
	case  TYPE::GHOST: return "Ghost";
	case  TYPE::DRAGON: return "Dragon";
	case  TYPE::DARK: return "Dark";
	case  TYPE::STEEL: return "Steel";
	case  TYPE::FAIRY: return "Fairy";
	}
}

// Intent:	change effect in int to string
// Pre:		int of type
// Post:	return string of type
std::string GameLibrary::changeToEffectName(int effectNum)
{
	switch (effectNum)
	{
	case EFFECT::NONE: return "";
	case  EFFECT::PAR: return "PAR";
	case  EFFECT::BUR: return "BRN";
	case  EFFECT::PSN: return "PSN";
	}
}