/*
 * main.cpp
 *
 *  Created on: 02.10.2011
 *      Author: brainstar
 */

#include "Game.h"
#include <iostream>
#include <SDL/SDL.h>
using namespace std;

int main(int arg, char *argv[]) {
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL couldn't be initialized: %s\n", SDL_GetError());
		exit(0);
	}
	atexit(SDL_Quit);

	SDL_Surface *screen;

	screen = SDL_SetVideoMode(600, 600, 16, SDL_SWSURFACE);
	if (NULL == screen) {
		fprintf(stderr, "Couldn't open window: %s\n", SDL_GetError());
		exit(0);
	}

	Game *game = new Game(screen);
	game->start();

	return 1;
}
