/*
 * Game.h
 *
 *  Created on: 02.10.2011
 *      Author: brainstar
 */

#ifndef GAME_H_
#define GAME_H_

#define GAME_PORT 9559

#include <SDL/SDL.h>
using namespace std;

class Game
{
public:
	enum Role {
		LOCAL,
		CLIENT,
		SERVER
	};

	Game();
	Game(SDL_Surface *screen, Role role);
	~Game();

	int start();
	int setPlayers(int i);
	bool isLocal();
	bool isClient();
	bool isServer();

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

	void init(SDL_Surface *screen, Role role);
	void reset();


	Role role;
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
