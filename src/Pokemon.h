/***********************************************************************
 * File: Pokemon.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for Pokemon class header
***********************************************************************/
#pragma once
#include <string>
#include <vector>
#include "Enums.h"
using namespace std;

class Move;
class GameLibrary;

class Pokemon
{
private:
	string name;
	vector<int> type;
	int maxHp;
	int currentHp;
	int level = 50;
	int atk;
	int def;
	int spAtk;
	int spDef;
	int speed;
	vector<Move*> moves;
	vector<bool> status = { false, false, false, false };

public:
	Pokemon() {}
	Pokemon(const string& _name, const vector<int>& _type, const int& _hp, const int& _atk, const int& _def,
		const int& _spAtk, const int& _spDef, const  int& _speed) : name(_name), type(_type), maxHp(_hp), currentHp(_hp), atk(_atk),
		def(_def), spAtk(_spAtk), spDef(_spDef), speed(_speed) {}
	Pokemon(const Pokemon& other);
	~Pokemon()
	{
		for (auto& i : moves) delete(i);
	}

	void addMove(const GameLibrary& gLib, string name);

	// Intent:	upgrade
	// Pre:		None
	// Post:	level up
	void upgrade()
	{
		level++;
	}

	// Intent:	get damage
	// Pre:		amount of damage
	// Post:	None
	void getDamage(const int& damage) 
	{
		currentHp -= damage;
	}

	// Intent:	get addition effect
	// Pre:		type of addition effect
	// Post:	None
	void getAdditionEffect(const int& type) 
	{
		// if paralized cause speed half
		if (type == PAR && !status[PAR]) speed /= 2;

		status[type] = true;
	}

	// Intent:	remove addition effect
	// Pre:		type of addition effect
	// Post:	None
	void removeAdditionEffect(const int& type)
	{
		// if no longer paralize cause speed double
		if (type == PAR && status[PAR]) speed *= 2;

		status[type] = false;
	}

	// Intent:	heal
	// Pre:		amount of heal
	// Post:	None
	void healing(const int& points)
	{
		this->currentHp += points;

		// if over max hp
		if (this->currentHp > this->maxHp) this->currentHp = this->maxHp;
	}

	//getter and setter....................................................
	const string& getName() { return name; }

	const vector<int>& getType() { return type; }

	const int& getCurrentHp() { return currentHp; }
	const int& getMaxHp() { return maxHp; }
	const int& getLevel() { return level; }
	const int& getAtk() { return atk; }
	const int& getDef() { return def; }
	const int& getSpAtk() { return spAtk; }
	const int& getSpDef() { return spDef; }
	const int& getSpeed() { return speed; }
	const vector<bool>& getStatus() { return status; }
	void setStatus(const int& type, const bool& flag) { status[type] = flag; }

	vector<Move*>& getMoves() { return moves; }
};