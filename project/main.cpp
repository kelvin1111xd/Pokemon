/***********************************************************************
 * File: main.cpp
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for Pokemon Gmae
***********************************************************************/
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "PokemonSystem.h"
#include "Enums.h"
#include "GameLibrary.h"
#include "BattleManager.h"
#include "Player.h"
#include "Pokemon.h"
#include "Move.h"
#include <conio.h>
#include <ctime>
using namespace std;

int main()
{
	srand(time(NULL));
	// open game wedsite
	system("start PokemonGame.html");

	// create game and executive
	PokemonSystem pokemonSystem;
	pokemonSystem.exe();
	return 0;
}

