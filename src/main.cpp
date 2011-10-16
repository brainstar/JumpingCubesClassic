/*
 * main.cpp
 *
 *  Created on: 02.10.2011
 *      Author: brainstar, blacktremolo
 */

#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>

#include "Game.h"
#include "Network.h"

using namespace std;

int main(int arg, char *argv[])
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL couldn't be initialized: %s\n", SDL_GetError());
		exit(0);
	}
	atexit(SDL_Quit);
	atexit(SDLNet_Quit);
	// Note: atexit functions get called in reversed order
	// (i.e. SDLNet_Quit gets called first, then SDL_Quit)
	// Dunno if the order is even important in our case =D

	// Setup networking if desired
	bool isNetworkGame = false;
	char roleInput = 'X'; // X marks the spot :) [to be exact: the spot of fail]
	NetRole role = LOCAL;
	cout << "Local, client or server?: (l/c/s)   ";
	cin >> roleInput;
	switch(roleInput) {
		case 'l':
			role = LOCAL;
			isNetworkGame = false;
			break;
		case 'c':
			role = CLIENT;
			isNetworkGame = true;
			break;
		case 's':
			role = SERVER;
			isNetworkGame = true;
			break;
		default:
			cerr << "Invaild selection\n";
			exit(0);
			break;
	}

	GameNetworking *networking = new GameNetworking(role);

	/*
	  Gathering player amount is the same way for local and server role
	  but different way for client role
	*/
	int players = -1;
	if(role == LOCAL)
	{
		cout << "Please enter amount of players: (2 - 4)   ";
		cin >> players;
		if(!(players == 0 || (players >= 2 && players <= 4)))
		{
			cerr << "Invalid selection\n";
			exit(0);
		}
	}
	else {
		players = networking->getPlayers();
	}

	int scrRes;
	scrRes = 500;

	SDL_Surface *screen;
	screen = SDL_SetVideoMode(scrRes, scrRes, 16, SDL_SWSURFACE);
	
	if (NULL == screen)
	{
		fprintf(stderr, "Couldn't open window: %s\n", SDL_GetError());
		exit(0);
	}

	SDL_WM_SetCaption("Jumping Cubes Classic", 0);

	Game *game = new Game(screen, networking);
	game->setPlayers(players);
	game->start();

	delete game;
	SDLNet_Quit();
	SDL_Quit();

	return 1;
}
