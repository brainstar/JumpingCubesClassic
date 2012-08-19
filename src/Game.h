/*
 * Game.h
 *
 *  Created on: 02.10.2011
 *      Author: Christian M.
 */

#ifndef GAME_H_
#define GAME_H_

#include <list>
#include "JCCTypes.h"
#include "Renderer.h"
using namespace std;

class Game
{
public:
	Game (unsigned int players, unsigned int fieldSize, Renderer* r);
	~Game();

	// Set Renderer interface
	Renderer* setRenderer(Renderer* r);

	// Make a move
	int move(int x, int y);
	// Get actual map
	Map getActualMap();
	vector<int> getStats();

private:
	// Reset the game
	int reset(int p, int s);

	Map map; // Map
	list<vector<int> > iRollMap;
	int maxcount; // Size of the field
	
	std::vector<int> player; // Vector that holds the actual points of each player
	int currentPlayer; // Int that holds the ID of player-in-turn
	int players; // Amount of players
	
	Renderer* renderer; // The interface to the display
	
	// Change the owner of a field
	void changeOwner(Field *f, int p);

	// Returns ID of winning player, or 0 if none
	int over();
	// Sets ID to the next player
	void next();

	vector<int> getVector(int x, int y);
	void increment(int x, int y);
};

#endif /* GAME_H_ */
