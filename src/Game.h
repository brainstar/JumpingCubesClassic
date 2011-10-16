/*
 * Game.h
 *
 *  Created on: 02.10.2011
 *      Author: brainstar
 */

#ifndef GAME_H_
#define GAME_H_

#include <SDL/SDL.h>
#include "Network.h"

#define GAME_PORT 9559

using namespace std;

class Game
{
public:
	Game();
	Game(SDL_Surface *screen, GameNetworking *networking);
	~Game();

	int start();
	int setPlayers(int i);

private:
	SDL_Surface *screen;
	int scrLength, fieldLength;
	int field[10][10];
	int owner[10][10];
	int surrounding[10][10];
	int player[5];
	int currentPlayer;
	int players;
	unsigned int black, grey, semiblack;
	unsigned int colors[9];

	NetRole role;
	GameNetworking *networking;

	void init(SDL_Surface *screen, GameNetworking *networking);
	void reset();

	int startRandom();
	int startLocal();

	void draw();
	int move(int x, int y);
	int roll(int x, int y);
	int over();
	void next();
	void updateCaption();
};

#endif /* GAME_H_ */
