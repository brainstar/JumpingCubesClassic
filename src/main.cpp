/*
 * main.cpp
 *
 *  Created on: 02.10.2011
 *      Author: brainstar, blacktremolo
 */

#include "Game.h"
#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_net.h>
using namespace std;

void netInit() {
	if(SDLNet_Init() == -1) {
		fprintf(stderr, "SDL Net couldn't be initialized: %s\n", SDLNet_GetError());
		exit(0);
	}
}

int main(int arg, char *argv[])
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL couldn't be initialized: %s\n", SDL_GetError());
		exit(0);
	}
	atexit(SDL_Quit);

	// Setup networking if desired
	bool isNetworkGame = false;
	char roleInput = 'X'; // X marks the spot :)
	Game::Role role = Game::LOCAL;
	cout << "Local, client or server?: (l/c/s)   ";
	cin >> roleInput;
	switch(roleInput) {
		case 'l':
			role = Game::LOCAL;
			isNetworkGame = false;
			break;
		case 'c':
			role = Game::CLIENT;
			isNetworkGame = true;
			netInit();
			break;
		case 's':
			role = Game::SERVER;
			isNetworkGame = true;
			netInit();
			break;
		default:
			cerr << "Invaild selection\n";
			exit(0);
			break;
	}

	IPaddress ipaddress;

	/*
	  Gathering player amount is the same way for local and server role
	  but different way for client role
	*/
	int players;
	if(role != Game::CLIENT)
	{
		cout << "Please enter amount of players: (2 - 4)   ";
		cin >> players;
	}
	else {
		if(SDLNet_ResolveHost(&ipaddress, "localhost", GAME_PORT) == -1) {
			fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
			exit(1);
		}
		
		TCPsocket playerSocket = SDLNet_TCP_Open(&ipaddress);
		if(!playerSocket) {
			fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			exit(0);
		}
		/*
		  TODO: Make the client wait and listen for the player amount
		        and possibly other data
		*/
	}

	if(role == Game::SERVER) {
		TCPsocket serverSocket;
		TCPsocket clientSockets[players-1];
		int numConnected = 0;

		if(SDLNet_ResolveHost(&ipaddress, NULL, GAME_PORT) == -1) {
    			fprintf(stderr, "SDL Net ResolveHost failed: %s\n", SDLNet_GetError());
    			exit(0);
		}

		serverSocket = SDLNet_TCP_Open(&ipaddress);
		if(!serverSocket) {
   			printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
   			exit(0);
		}

		while(numConnected < players)
		{
			TCPsocket clientSocket = SDLNet_TCP_Accept(serverSocket);
			if(clientSocket)
			{
				clientSockets[numConnected] = clientSocket;
				numConnected++;
				// TODO: Inform client about player number
			}
		}
		// TODO: Send some "ready" message to clients
	}

			cerr << "This amount of players is invalid!";
			exit(0);
	}
	

	SDL_Surface *screen;

	int scrRes;
	scrRes = 500;

	screen = SDL_SetVideoMode(scrRes, scrRes, 16, SDL_SWSURFACE);
	if (NULL == screen)
	{
		fprintf(stderr, "Couldn't open window: %s\n", SDL_GetError());
		exit(0);
	}

	SDL_WM_SetCaption("Jumping Cubes Classic", 0);

	Game *game = new Game(screen, role);
	game->setPlayers(players);
	game->start();

	SDLNet_Quit();
	SDL_Quit();
	delete game;
	SDL_Quit();

	return 1;
}
