/*
 * Game.h
 *
 *  Created on: 02.10.2011
 *      Author: brainstar
 */

#ifndef GAME_H_
#define GAME_H_

#include <SDL/SDL.h>
using namespace std;

class Game {
public:
	Game();
	Game(SDL_Surface *screen);
	~Game();

	void start();

private:
	SDL_Surface *screen;
	int field[10][10];
	int owner[10][10];
	int surrounding[10][10];
	int player[5];
	int currentPlayer;
	unsigned int black, grey, red, green, blue, yellow;

	void draw();
	int move(int x, int y);
	int roll(int x, int y);
};

#endif /* GAME_H_ */
