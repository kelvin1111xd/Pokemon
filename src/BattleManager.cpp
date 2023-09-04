/***********************************************************************
 * File: BattleManager.cpp
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for BattleManager class cpp
***********************************************************************/
#include "BattleManager.h"
#include "Player.h"
#include "Pokemon.h"
#include "Move.h"
#include <iostream>
using namespace std;

// Intent:	use move
// Pre:		player's move and oponent's move
// Post:	both player attack
void BattleManager::move(const string& playerMove, const string& oponentMove)
{
	// compute who move first
	Pokemon* first, * second;
	string firstMove, secondMove;
	bool firstIsOponentPokemon = false, firstFaited = false, secondFaited = false;

	if (player->getPokemons()[player->getCurrentPokemon()]->getSpeed() >= oponent->getPokemons()[oponent->getCurrentPokemon()]->getSpeed())
	{
		first = player->getPokemons()[player->getCurrentPokemon()];
		firstMove = playerMove;
		second = oponent->getPokemons()[oponent->getCurrentPokemon()];
		secondMove = oponentMove;
	}
	else
	{
		second = player->getPokemons()[player->getCurrentPokemon()];
		secondMove = playerMove;
		first = oponent->getPokemons()[oponent->getCurrentPokemon()];
		firstMove = oponentMove;
		firstIsOponentPokemon = true;
	}

	// first attack
	if (!first->getStatus()[PAR]) // if attacker not in paralized
	{
		for (auto i : first->getMoves())
			if (i->getName() == firstMove)
			{
				// used move
				i->used();

				// output message
				if (firstIsOponentPokemon) log.push_back(pair<int, string>{ATTACK, "The opposing " + first->getName() + " used " + i->getName() + "!"});
				else log.push_back(pair<int, string>{ATTACK, first->getName() + " used " + i->getName() + "!"});
				cout << "[Turn " << turn << "] " << log.back().second << endl;
				message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				
				// avoid attack
				if (!testMode && rand() % 100 > i->getAccuracy())
				{
					log.push_back(pair<int, string>{AVOIDED, second->getName() + " avoided the attack!"});
					if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					break;
				}
				
				// addition effect
				if (i->getAdditionEffect() != NONE)
				{
					// add addtion effect
					second->getAdditionEffect(i->getAdditionEffect());

					// output message
					if (i->getAdditionEffect() == PAR) // if paralized
						log.push_back(pair<int, string>{PARALYZED, second->getName() + " is paralyzed, so it may be unable to move!"});
					else if (i->getAdditionEffect() == BUR) // if burned
						log.push_back(pair<int, string>{BURN, second->getName() + " was burned!"});
					else if (i->getAdditionEffect() == PSN) // if poisoned
						log.push_back(pair<int, string>{POISON, second->getName() + " was posined!"});

					if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;

					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				// count damage
				int damage;
				if (!(i->getSpecialAttack()) && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : first->getType())
						if (i->getType() == j) stab = true;

					//compute type effectiveness
					double type = typeEffectiveness(i->getType(), second->getType());

					int level = first->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8)
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damge calculate
					damage = damageCal(level, i->getPower(), first->getAtk(), second->getDef(), isCritical, stab, type);

					// get damage
					second->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				else if (i->getSpecialAttack() && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : first->getType())
						if (i->getType() == j) stab = true;

					// compute typed effectiveness
					double type = typeEffectiveness(i->getType(), second->getType());

					int level = first->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8)
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damage calculate
					damage = damageCal(level, i->getPower(), first->getSpAtk(), second->getSpDef(), isCritical, stab, type);

					// get damage
					second->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				break;
			}
	}
	else // if attacker in paralized
	{
		// output message
		if (firstIsOponentPokemon) log.push_back(pair<int, string>{EFFECTED, "The opposing " + first->getName() + " is paralyzed!\n[Turn " + to_string(turn) + "] It can't move!" });
		else log.push_back(pair<int, string>{ATTACK, first->getName() + " is paralyzed!\n[Turn " + to_string(turn) + "] It can't move!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	
	// if faited
	if (!firstIsOponentPokemon)
	{
		if (second->getCurrentHp() <= 0)
		{
			// output message
			log.push_back(pair<int, string>{FAITED, "The opposing " + second->getName() + " faited!"});
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

			// change pokemon
			if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size())
			{
				oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);
			}

			gameState = checkGameState();

			if (gameState == 0)
			{
				// addition effect
				if (first->getStatus()[BUR])
				{
					log.push_back(pair<int, string>{BURN, first->getName() + " is hurt by its burn!"});
					if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					first->getDamage(first->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}
				if (first->getStatus()[PSN])
				{
					log.push_back(pair<int, string>{BURN, first->getName() + " is hurt by its poisoning!"});
					if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					first->getDamage(first->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				if (first->getCurrentHp() <= 0)
				{
					log.push_back(pair<int, string>{FAITED, first->getName() + " faited!"});
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					gameState = checkGameState();
				}
			}			

			turn++;
			return;
		}
	}
	else
	{
		if (second->getCurrentHp() <= 0)
		{
			// output message
			secondFaited = true;
			log.push_back(pair<int, string>{FAITED, second->getName() + " faited!"});
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

			gameState = checkGameState();

			if (gameState == 0) 
			{
				// addition effect
				if (first->getStatus()[BUR])
				{
					log.push_back(pair<int, string>{BURN, first->getName() + " is hurt by its burn!"});
					if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					first->getDamage(first->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}
				if (first->getStatus()[PSN])
				{
					log.push_back(pair<int, string>{BURN, first->getName() + " is hurt by its poisoning!"});
					if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					first->getDamage(first->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				if (first->getCurrentHp() <= 0)
				{
					log.push_back(pair<int, string>{FAITED, "The opposing " + first->getName() + " faited!"});
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					gameState = checkGameState();
				}
			}
			
			turn++;
			return;
		}
	}

	// second attack
	if (!second->getStatus()[PAR]) // if attacker not in paralized
	{
		for (auto i : second->getMoves())
			if (i->getName() == secondMove)
			{
				// used move
				i->used();

				// output message
				if (!firstIsOponentPokemon) log.push_back(pair<int, string>{ATTACK, "The opposing " + second->getName() + " used " + i->getName() + "!"});
				else log.push_back(pair<int, string>{ATTACK, second->getName() + " used " + i->getName() + "!"});
				cout << "[Turn " << turn << "] " << log.back().second << endl;
				message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

				// avoid attack
				if (!testMode && rand() % 100 > i->getAccuracy())
				{
					log.push_back(pair<int, string>{AVOIDED, first->getName() + " avoided the attack!"});
					if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					break;
				}

				// addition effect
				if (i->getAdditionEffect() != NONE)
				{
					// add addtion effect
					first->getAdditionEffect(i->getAdditionEffect());

					// output message
					if (i->getAdditionEffect() == PAR) // if paralized
						log.push_back(pair<int, string>{PARALYZED, first->getName() + " is paralyzed, so it may be unable to move!"});
					else if (i->getAdditionEffect() == BUR) // if burned
						log.push_back(pair<int, string>{BURN, first->getName() + " was burned!"});
					else if (i->getAdditionEffect() == PSN) // if poisoned
						log.push_back(pair<int, string>{POISON, first->getName() + " was posined!"});

					if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;

					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				// count damage
				int damage;
				if (!(i->getSpecialAttack()) && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : second->getType())
						if (i->getType() == j) stab = true;

					// compute type effectiveness
					double type = typeEffectiveness(i->getType(), first->getType());

					int level = second->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8) 
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damage calculate
					damage = damageCal(level, i->getPower(), second->getAtk(), first->getDef(), isCritical, stab, type);

					// get damage
					first->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				else if (i->getSpecialAttack() && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : second->getType())
						if (i->getType() == j) stab = true;

					// compute type effectiveness
					double type = typeEffectiveness(i->getType(), first->getType());

					int level = second->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8)
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damage calculate
					damage = damageCal(level, i->getPower(), second->getSpAtk(), first->getSpDef(), isCritical, stab, type);

					// get damage
					first->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				break;
			}
	}
	else // if attacker in paralized
	{
		// output message
		if (!firstIsOponentPokemon) log.push_back(pair<int, string>{EFFECTED, "The opposing " + second->getName() + " is paralyzed!\n[Turn " + to_string(turn) + "] It can't move!" });
		else log.push_back(pair<int, string>{ATTACK, second->getName() + " is paralyzed!\n[Turn " + to_string(turn) + "] It can't move!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	// if faited
	if (!firstIsOponentPokemon)
	{
		if (first->getCurrentHp() <= 0)
		{
			// output message
			log.push_back(pair<int, string>{FAITED, first->getName() + " faited!"});
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

			gameState = checkGameState();

			if (gameState == 0) 
			{
				// addition effect
				if (second->getStatus()[BUR])
				{
					log.push_back(pair<int, string>{BURN, second->getName() + " is hurt by its burn!"});
					if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					second->getDamage(second->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}
				if (second->getStatus()[PSN])
				{
					log.push_back(pair<int, string>{BURN, second->getName() + " is hurt by its poisoning!"});
					if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					second->getDamage(second->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				if (second->getCurrentHp() <= 0)
				{
					log.push_back(pair<int, string>{FAITED, "The opposing " + second->getName() + " faited!"});
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					gameState = checkGameState();
				}
			}
			
			turn++;
			return;
		}
	}
	else
	{
		if (first->getCurrentHp() <= 0)
		{
			// output message
			log.push_back(pair<int, string>{FAITED, "The opposing " + first->getName() + " faited!"});
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

			// change pokemon
			if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size()) oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);
			
			gameState = checkGameState();

			if (gameState == 0) 
			{
				// addition effect
				if (second->getStatus()[BUR])
				{
					log.push_back(pair<int, string>{BURN, second->getName() + " is hurt by its burn!"});
					if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					second->getDamage(second->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}
				if (second->getStatus()[PSN])
				{
					log.push_back(pair<int, string>{BURN, second->getName() + " is hurt by its poisoning!"});
					if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
					second->getDamage(second->getMaxHp() / 16);
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				if (second->getCurrentHp() <= 0)
				{
					log.push_back(pair<int, string>{FAITED, second->getName() + " faited!"});
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

			}
			
			turn++;
			return;
		}
	}

	// addition effect
	if (first->getStatus()[BUR])
	{
		log.push_back(pair<int, string>{BURN, first->getName() + " is hurt by its burn!"});
		if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
		first->getDamage(first->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (first->getStatus()[PSN])
	{
		log.push_back(pair<int, string>{BURN, first->getName() + " is hurt by its poisoning!"});
		if (firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
		first->getDamage(first->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	if (second->getStatus()[BUR])
	{
		log.push_back(pair<int, string>{BURN, second->getName() + " is hurt by its burn!"});
		if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
		second->getDamage(second->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (second->getStatus()[PSN])
	{
		log.push_back(pair<int, string>{BURN, second->getName() + " is hurt by its poisoning!"});
		if (!firstIsOponentPokemon) log.back().second = "The opposing " + log.back().second;
		second->getDamage(second->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	// if faited
	if (first->getCurrentHp() <= 0)
	{
		log.push_back(pair<int, string>{FAITED, first->getName() + " faited!"});

		if (firstIsOponentPokemon)
		{
			log.back().second = "The opposing " + log.back().second;

			if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size()) oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);
		}
				
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	if (second->getCurrentHp() <= 0)
	{
		log.push_back(pair<int, string>{FAITED, second->getName() + " faited!"});

		if (!firstIsOponentPokemon)
		{
			log.back().second = "The opposing " + log.back().second;

			if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size()) oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);
		}

		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	turn++;
}

// Intent:	use swap
// Pre:		index of pokemon which wan't to swap and oponent's move
// Post:	change current pokemon and oponent attack
void BattleManager::swap(const int& swapIndex, const string& oponentMove)
{
	// get data and change current pokemon
	string currentPokemonName = player->getPokemons()[player->getCurrentPokemon()]->getName();
	player->setCurrentPokemon(swapIndex);
	Pokemon* playerPokemon = player->getPokemons()[player->getCurrentPokemon()];
	Pokemon* oponentPokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];

	// output message
	log.push_back(pair<int, string>{SWAP, currentPokemonName + ", switch out!\n[Turn " + to_string(turn) + "] Come back!\n[Turn " + to_string(turn) + "] Go!" + playerPokemon->getName() + "!"});
	cout << "[Turn " << turn << "] " << log.back().second << endl;
	message += "[Turn " + to_string(turn) + "] " + currentPokemonName + ", switch out!\n";
	message += "[Turn " + to_string(turn) + "] " + "Come back!\n";
	message += "[Turn " + to_string(turn) + "] " + "Go! " + playerPokemon->getName() + "!\n";

	// oponent attack
	if (!oponentPokemon->getStatus()[PAR]) // if not in paralized
	{
		for (auto i : oponentPokemon->getMoves())
			if (i->getName() == oponentMove)
			{
				// used move
				i->used();

				// output message
				log.push_back(pair<int, string>{ATTACK, "The opposing " + oponentPokemon->getName() + " used " + i->getName() + "!"});
				cout << "[Turn " << turn << "] " << log.back().second << endl;
				message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

				// avoid attack
				if (!testMode && rand() % 100 > i->getAccuracy())
				{
					log.push_back(pair<int, string>{AVOIDED, playerPokemon->getName() + " avoided the attack!"});
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					break;
				}

				// addition effect
				if (i->getAdditionEffect() != NONE)
				{
					// add addition effect
					playerPokemon->getAdditionEffect(i->getAdditionEffect());

					// output message
					if (i->getAdditionEffect() == PAR)
						log.push_back(pair<int, string>{PARALYZED, playerPokemon->getName() + " is paralyzed, so it may be unable to move!"});
					else if (i->getAdditionEffect() == BUR)
						log.push_back(pair<int, string>{BURN, playerPokemon->getName() + " was burned!"});
					else if (i->getAdditionEffect() == PSN)
						log.push_back(pair<int, string>{POISON, playerPokemon->getName() + " was posined!"});

					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				// count damage
				int damage;
				if (!(i->getSpecialAttack()) && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : oponentPokemon->getType())
						if (i->getType() == j) stab = true;

					// compute type effectiveness
					double type = typeEffectiveness(i->getType(), playerPokemon->getType());

					int level = oponentPokemon->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8)
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damge calculate
					damage = damageCal(level, i->getPower(), oponentPokemon->getAtk(), playerPokemon->getDef(), isCritical, stab, type);

					// get damage
					playerPokemon->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				else if (i->getSpecialAttack() && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : oponentPokemon->getType())
						if (i->getType() == j) stab = true;

					// compute type effectiveness
					double type = typeEffectiveness(i->getType(), playerPokemon->getType());

					int level = oponentPokemon->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8)
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damage calculate
					damage = damageCal(level, i->getPower(), oponentPokemon->getSpAtk(), playerPokemon->getSpDef(), isCritical, stab, type);

					// get damage
					playerPokemon->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				break;
			}
	}
	else // if in paralized
	{
		// output message
		log.push_back(pair<int, string>{ATTACK, oponentPokemon->getName() + " is paralyzed!\n[Turn " + to_string(turn) + "] It can't move!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	// if faited
	if (playerPokemon->getCurrentHp() <= 0)
	{
		// output message
		log.push_back(pair<int, string>{FAITED, playerPokemon->getName() + " faited!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

		// addition effect
		if (oponentPokemon->getStatus()[BUR])
		{
			log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its burn!"});
			oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		}
		if (oponentPokemon->getStatus()[PSN])
		{
			log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its poisoning!"});
			oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		}

		if (oponentPokemon->getCurrentHp() <= 0)
		{
			log.push_back(pair<int, string>{FAITED, "The opposing " + oponentPokemon->getName() + " faited!"});

			if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size()) oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);

			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		}

		turn++;
		return;
	}

	// addition effect
	if (oponentPokemon->getStatus()[BUR])
	{
		log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its burn!"});
		oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (oponentPokemon->getStatus()[PSN])
	{
		log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its poisoning!"});
		oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	if (playerPokemon->getStatus()[BUR])
	{
		log.push_back(pair<int, string>{BURN, playerPokemon->getName() + " is hurt by its burn!"});
		playerPokemon->getDamage(playerPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (playerPokemon->getStatus()[PSN])
	{
		log.push_back(pair<int, string>{BURN, playerPokemon->getName() + " is hurt by its poisoning!"});
		playerPokemon->getDamage(playerPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	// if faited
	if (oponentPokemon->getCurrentHp() <= 0)
	{
		log.push_back(pair<int, string>{FAITED, "The opposing " + oponentPokemon->getName() + " faited!"});

		if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size()) oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);

		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (playerPokemon->getCurrentHp() <= 0)
	{
		log.push_back(pair<int, string>{FAITED, playerPokemon->getName() + " faited!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	turn++;
}

// Intent:	use potion
// Pre:		potion name, pokemon which use potion, oponent's move
// Post:	heal pokemon and oponent attack
void BattleManager::usePotion(const string& potionName, Pokemon& pokemon, const string& oponentMove)
{
	// get date and use potion
	Pokemon* playerPokemon = player->getPokemons()[player->getCurrentPokemon()];
	Pokemon* oponentPokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];

	int health = 0;

	if (potionName == "Potion")
	{
		pokemon.healing(20);
		if (pokemon.getMaxHp() - pokemon.getCurrentHp() <= 20) health = pokemon.getMaxHp() - pokemon.getCurrentHp();
		else health = 20;
	}
	else if (potionName == "Super Potion")
	{
		pokemon.healing(60);
		if (pokemon.getMaxHp() - pokemon.getCurrentHp() <= 60) health = pokemon.getMaxHp() - pokemon.getCurrentHp();
		else health = 60;
	}
	else if (potionName == "Hyper Potion")
	{
		pokemon.healing(120);
		if (pokemon.getMaxHp() - pokemon.getCurrentHp() <= 60) health = pokemon.getMaxHp() - pokemon.getCurrentHp();
		else health = 60;
	}
	else if (potionName == "Max Potion")
	{
		pokemon.healing(player->getPokemons()[player->getCurrentPokemon()]->getMaxHp());
		health = pokemon.getMaxHp() - pokemon.getCurrentHp();
	}

	// output message
	log.push_back(pair<int, string>{POISON, "You used a " + potionName + "!\n[Turn " + to_string(turn) + "] " + pokemon.getName() + " had its HP restored."});
	cout << "[Turn " << turn << "] " << log.back().second << endl;
	message += "[Turn " + to_string(turn) + "] " + "You used a " + potionName + "!\n";
	message += "[Turn " + to_string(turn) + "] " + pokemon.getName() + " had its HP restored." + "\n";

	// oponent attack
	if (!oponentPokemon->getStatus()[PAR]) // if not in paralized
	{
		for (auto i : oponentPokemon->getMoves())
			if (i->getName() == oponentMove)
			{
				// used move
				i->used();

				// output message
				log.push_back(pair<int, string>{ATTACK, "The opposing " + oponentPokemon->getName() + " used " + i->getName() + "!"});
				cout << "[Turn " << turn << "] " << log.back().second << endl;
				message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

				// avoid attack
				if (!testMode && rand() % 100 > i->getAccuracy())
				{
					log.push_back(pair<int, string>{AVOIDED, playerPokemon->getName() + " avoided the attack!"});
					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					break;
				}

				// addition effective
				if (i->getAdditionEffect() != NONE)
				{
					// add addition effect
					playerPokemon->getAdditionEffect(i->getAdditionEffect());

					// output message
					if (i->getAdditionEffect() == PAR)
						log.push_back(pair<int, string>{PARALYZED, playerPokemon->getName() + " is paralyzed, so it may be unable to move!"});
					else if (i->getAdditionEffect() == BUR)
						log.push_back(pair<int, string>{BURN, playerPokemon->getName() + " was burned!"});
					else if (i->getAdditionEffect() == PSN)
						log.push_back(pair<int, string>{POISON, playerPokemon->getName() + " was posined!"});

					cout << "[Turn " << turn << "] " << log.back().second << endl;
					message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
				}

				// count damage
				int damage;
				if (!(i->getSpecialAttack()) && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : oponentPokemon->getType())
						if (i->getType() == j) stab = true;

					// compute type effectiveness
					double type = typeEffectiveness(i->getType(), playerPokemon->getType());

					int level = oponentPokemon->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8)
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damage calculate
					damage = damageCal(level, i->getPower(), oponentPokemon->getAtk(), playerPokemon->getDef(), isCritical, stab, type);

					// get damage
					playerPokemon->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				else if (i->getSpecialAttack() && i->getPower() != 0)
				{
					// compute stab
					bool stab = false;
					for (auto j : oponentPokemon->getType())
						if (i->getType() == j) stab = true;

					// compute type effectiveness
					double type = typeEffectiveness(i->getType(), playerPokemon->getType());

					int level = oponentPokemon->getLevel();

					bool isCritical = false;
					if (rand() % 10 == 8)
					{
						isCritical = true;
						log.push_back(pair<int, string>{CRITICAL, "A critical hit!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}

					// damage calculate
					damage = damageCal(level, i->getPower(), oponentPokemon->getSpAtk(), playerPokemon->getSpDef(), isCritical, stab, type);

					// get damage
					playerPokemon->getDamage(damage);

					// output message
					if (abs(type - 2.0) < 0.0001)
					{
						log.push_back(pair<int, string>{SUPER, "It's super effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type - 0.5) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTVERY, "It's not very effective..."});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
					else if (abs(type) < 0.0001)
					{
						log.push_back(pair<int, string>{NOTEFF, "It's not effective!"});
						cout << "[Turn " << turn << "] " << log.back().second << endl;
						message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
					}
				}
				break;
			}
	}
	else // if in paralized
	{
		// output message
		log.push_back(pair<int, string>{ATTACK, oponentPokemon->getName() + " is paralyzed!\n[Turn " + to_string(turn) + "] It can't move!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	// if faited
	if (playerPokemon->getCurrentHp() <= 0)
	{
		// output message
		log.push_back(pair<int, string>{FAITED, playerPokemon->getName() + " faited!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";

		// addition effect
		if (oponentPokemon->getStatus()[BUR])
		{
			log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its burn!"});
			oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		}
		if (oponentPokemon->getStatus()[PSN])
		{
			log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its poisoning!"});
			oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		}

		// if faited
		if (oponentPokemon->getCurrentHp() <= 0)
		{
			log.push_back(pair<int, string>{FAITED, "The opposing " + oponentPokemon->getName() + " faited!"});

			if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size()) oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);

			cout << "[Turn " << turn << "] " << log.back().second << endl;
			message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		}

		turn++;
		return;
	}

	// addition effect
	if (oponentPokemon->getStatus()[BUR])
	{
		log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its burn!"});
		oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (oponentPokemon->getStatus()[PSN])
	{
		log.push_back(pair<int, string>{BURN, "The opposing " + oponentPokemon->getName() + " is hurt by its poisoning!"});
		oponentPokemon->getDamage(oponentPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	if (playerPokemon->getStatus()[BUR])
	{
		log.push_back(pair<int, string>{BURN, playerPokemon->getName() + " is hurt by its burn!"});
		playerPokemon->getDamage(playerPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (playerPokemon->getStatus()[PSN])
	{
		log.push_back(pair<int, string>{BURN, playerPokemon->getName() + " is hurt by its poisoning!"});
		playerPokemon->getDamage(playerPokemon->getMaxHp() / 16);
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	// if faited
	if (oponentPokemon->getCurrentHp() <= 0)
	{
		log.push_back(pair<int, string>{FAITED, "The opposing " + oponentPokemon->getName() + " faited!"});

		if (oponent->getCurrentPokemon() + 1 < oponent->getPokemons().size()) oponent->setCurrentPokemon(oponent->getCurrentPokemon() + 1);

		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}
	if (playerPokemon->getCurrentHp() <= 0)
	{
		log.push_back(pair<int, string>{FAITED, playerPokemon->getName() + " faited!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
	}

	turn++;
}

// Intent:	count type effectiveness
// Pre:		attacker move type and deffender types
// Post:	return type effectiveness multiple
double BattleManager::typeEffectiveness(const int& moveType, const vector<int>& type)
{
	// type chart
	vector<vector<double>> typeChart =
	{
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.0, 1.0, 1.0, 0.5, 1.0},
		{1.0, 0.5, 0.5, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 2.0, 1.0},
		{1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 2.0, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0}, 
		{1.0, 0.5, 2.0, 1.0, 0.5, 1.0, 1.0, 0.5, 2.0, 0.5, 1.0, 0.5, 2.0, 1.0, 0.5, 1.0, 0.5, 1.0}, 
		{1.0, 0.5, 0.5, 1.0, 2.0, 0.5, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0}, 
		{2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 0.5, 0.5, 0.5, 2.0, 0.0, 1.0, 2.0, 2.0, 0.5}, 
		{1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 0.0, 2.0}, 
		{1.0, 2.0, 1.0, 2.0, 0.5, 1.0, 1.0, 2.0, 1.0, 0.0, 1.0, 0.5, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0}, 
		{1.0, 1.0, 1.0, 0.5, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 0.0, 0.5, 1.0}, 
		{1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 0.5, 0.5, 1.0, 0.5, 2.0, 1.0, 1.0, 0.5, 1.0, 2.0, 0.5, 0.5}, 
		{1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0}, 
		{0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 0.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 0.5}, 
		{1.0, 0.5, 0.5, 0.5, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.5, 2.0}, 
		{1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 0.5, 1.0}
	};

	// count result of type effectiveness amount
	double result = 1.0;

	for (const auto& i : type)
	{
		result *= typeChart[moveType][i];
	}

	return result;
}

// Intent:	call if player's pokemon faited
// Pre:		index of pokemon which wan't to switch
// Post:	change current pokemon
void BattleManager::faited(const int& switchIndex)
{
	// switch current pokemon
	player->setCurrentPokemon(switchIndex);

	// output message
	Pokemon* playerPokemon = player->getPokemons()[player->getCurrentPokemon()];

	log.push_back(pair<int, string>{SWAP, "Go! " + playerPokemon->getName() + "!"});
	cout << "[Turn " << turn << "] " << log.back().second << endl;
	message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
}

// Intent:	check game state
// Pre:		None
// Post:	None
int BattleManager::checkGameState()
{
	// check if player's all pokemon faited
	bool allPokemonFaited = true;
	for (int i = 0; i < player->getPokemons().size(); i++)
	{
		if (player->getPokemons()[i]->getCurrentHp() > 0) allPokemonFaited = false;
	}

	// output message
	if (allPokemonFaited)
	{
		log.push_back(pair<int, string>{GAMEOVER, "You Loss!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		return 2;
	}

	// check if oponent's all pokemon faited
	allPokemonFaited = true;
	for (int i = 0; i < oponent->getPokemons().size(); i++)
	{
		if (oponent->getPokemons()[i]->getCurrentHp() > 0) allPokemonFaited = false;
	}

	// output message
	if (allPokemonFaited)
	{
		log.push_back(pair<int, string>{GAMEOVER, "You Win!"});
		cout << "[Turn " << turn << "] " << log.back().second << endl;
		message += "[Turn " + to_string(turn) + "] " + log.back().second + "\n";
		return 1;
	}

	return 0;
}

// Intent:	damage calculate
// Pre:		attacker level, move power, attacker atk, deffender def, critical, stab, type effectiveness
// Post:	return result damage
int BattleManager::damageCal(int level, int power, int a, int d, bool critical, bool stab, double type)
{
	// if test move
	if (testMode) critical = false;

	// count result of damage
	double result = (((double)(2 * level + 10) / 250) * power * a / d) + 2;

	if (critical) result *= 1.5;
	if (stab) result *= 1.5;

	result *= type;

	return (int)result;
}