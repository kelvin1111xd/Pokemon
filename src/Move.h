/***********************************************************************
 * File: Move.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for Move class header
***********************************************************************/
#pragma once
#include <string>
#include "Enums.h"
using namespace std;

class Move
{
private:
	string name;
	int type;
	int power;
	int accuracy;
	int pp;
	bool specialAttack;
	int additionEffect;

public:
	Move() {}
	Move(const string& _name, const int& _type, const int& _power, const int& _accuracy, const int& _pp,
		const bool& _specialAttack, const int& _additionEffect) : name(_name), type(_type), power(_power),
		accuracy(_accuracy), pp(_pp), specialAttack(_specialAttack), additionEffect(_additionEffect) {}

	// Intent:	when used move do subtract to pp
	// Pre:		None
	// Post:	None
	void used() { pp--; }

	//getter and setter..............................................
	const string& getName() { return name; }

	const int& getType() { return type; }
	const int& getPower() { return power; }
	const int& getAccuracy() { return accuracy; }
	const int& getPp() { return pp; }
	const bool& getSpecialAttack() { return specialAttack; }
	const int& getAdditionEffect() { return additionEffect; }
};

