/***********************************************************************
 * File: Enum.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for Enumeration
 ***********************************************************************/
#pragma once
#include <string>

enum TYPE
{
	NORMAL = 0,
	FIRE = 1,
	WATER = 2,
	ELECTRIC = 3,
	GRASS = 4,
	ICE = 5,
	FIGHTING = 6,
	POISON = 7,
	GROUND = 8,
	FLYING = 9,
	PSYCHIC = 10,
	BUG = 11,
	ROCK = 12,
	GHOST = 13,
	DRAGON = 14,
	DARK = 15,
	STEEL = 16,
	FAIRY = 17
};

enum EFFECT
{
	NONE = 0,
	PAR = 1,
	BUR = 2,
	PSN = 3
};

enum LOG
{
	ATTACK = 0,
	SUPER = 1,
	NOTVERY = 2,
	NOTEFF = 3,
	AVOIDED = 4,
	CRITICAL = 5,
	EFFECTED = 6,
	PARALYZED = 7,
	BURN = 8,
	POISONED = 9,
	POTION = 10,
	SWAP = 11,
	FAITED = 12,
	FAITEDSWAP = 13,
	GAMEOVER = 14
};
