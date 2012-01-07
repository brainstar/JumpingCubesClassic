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
	Game();
	~Game();

	int setPlayers(int i);
	int setFieldSize(int a);
	int setRenderer(Renderer* r);
	
	int startGame(int players = 0, int fieldSize = 8, Renderer* r = 0);
	void reset();
	
	int move(float x, float y); // Coordinates in [0.0, 1.0[

private:
	Map map; // Map
	vector<vector<bool> > rollMap; // Map of elements that roll over in the next iteration
//	int size; // Size of the field
	
	int player[5]; // Array that holds the actual points of each player
	int currentPlayer;
	int players; // Amount of players
	
	Renderer* renderer; // The interface to the display
	
	bool running; // Game is running?

	void changeOwner(Field *f, int p);

	int over();
	void next();
};

#endif /* GAME_H_ */
