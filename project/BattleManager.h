/***********************************************************************
 * File: BattleManager.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for BattleManager class header
***********************************************************************/
#pragma once
#include "Enums.h"
#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include <conio.h>
using namespace std;

class Player;
class Pokemon;
class Move;

class BattleManager
{
private:
	int turn = 1;
	vector<pair<int, string>> log;
	Player* player;
	Player* oponent;
	string message = "";
	bool testMode = false;
	int gameState;

public:
	BattleManager() {}
	BattleManager(Player* _player, Player* _oponent) : player(_player), oponent(_oponent) {}
	~BattleManager()
	{
		delete(player);
		delete(oponent);
	}

	void move(const string& playerMove, const string& oponentMove);

	void faited(const int& switchIndex);

	void swap(const int& swapIndex, const string& oponentMove);

	int checkGameState();

	void usePotion(const string& potionName, Pokemon& pokemon, const string& oponentMove);

	double typeEffectiveness(const int& moveType, const vector<int>& type);

	int damageCal(int level, int power, int a, int d, bool critical, bool stab, double type);

	void clearMessage() { message = ""; }

	//getter and setter........................................................
	void setTestMode() { testMode = true; }

	const int& getTurn() { return turn; }

	const vector<pair<int, string>>& getLog() { return log; }

	const string& getMessage() { return message; }

	const int& getGameState() { return gameState; }

	Player* getPlayer() { return player; }
	Player* getOponent() { return oponent; }
};

