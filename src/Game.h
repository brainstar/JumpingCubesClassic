/*
 * Game.h
 *
 *  Created on: 02.10.2011
 *      Author: Christian M.
 */

#ifndef GAME_H_
#define GAME_H_

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
	vector<vector<bool> > rollMap; // Map of elements that roll over in the next iteration
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
};

#endif /* GAME_H_ */
