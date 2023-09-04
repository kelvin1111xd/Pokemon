/***********************************************************************
 * File: PokemonSystem.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for PokemonSystem class header
***********************************************************************/
#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <vector>
#include <map>
#include "PokemonSystem.h"
#include "Enums.h"
#include "GameLibrary.h"
#include "BattleManager.h"
#include "Player.h"
#include "Pokemon.h"
#include "Move.h"

using namespace std;
using namespace web;
using namespace http;
using namespace http::experimental::listener;
using namespace utility::conversions;

class PokemonSystem
{
private:
    http_listener* listener;
    GameLibrary *gLib;
    Player* player;
    Player* oponent;
    BattleManager* battle;
    vector<json::value> pokemonDatas;
    ofstream outputFile;
public:
    PokemonSystem();

    void exe();  

    void readFile(string testCaseFileName, string pokemonFileName, string moveFileNmae, string gameDataFileName);

    void readCase(const string& caseFileName);
  
};

