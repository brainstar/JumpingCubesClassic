/*
 * Network.cpp
 *
 *  Created on: 11.10.2011
 *      Author: brainstar
 */

#include <iostream>
#include "Network.h"

void GameNetworking::initLib()
{
	if(SDLNet_Init() == -1) {
			fprintf(stderr, "SDL Net couldn't be initialized: %s\n", SDLNet_GetError());
			exit(0);
	}
}

GameNetworking::GameNetworking(NetRole role)
{
	this->role = role;

	initLib();	

	switch(role)
	{
		case CLIENT:
			if(SDLNet_ResolveHost(&this->ipaddress, "localhost", GAME_PORT) == -1) {
					fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
					exit(1);
				}
		
				this->clientConnectedSocket = SDLNet_TCP_Open(&ipaddress);
				if(!this->clientConnectedSocket) {
					fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
					exit(0);
				}
				/*
				  TODO: Make the CLIENT wait and listen for the player amount
					and possibly other data
				*/
			break;

		case SERVER:
			this->players = -1;
			cout << "Please enter amount of players: (2 - 4)   ";
			cin >> players;
			if(!(this->players == 0) || (this->players < 2 && this->players > 4))
			{
				cerr << "Invaild selection\n";
				exit(0);
			}

			this->numConnected = 0;
			if(SDLNet_ResolveHost(&ipaddress, NULL, GAME_PORT) == -1) {
	    			fprintf(stderr, "SDL Net ResolveHost failed: %s\n", SDLNet_GetError());
	    			exit(0);
			}

			this->serverSocket = SDLNet_TCP_Open(&ipaddress);
			if(!this->serverSocket) {
	   			printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
	   			exit(0);
			}

			while(this->numConnected < players)
			{
				TCPsocket clientSocket = SDLNet_TCP_Accept(this->serverSocket);
				if(clientSocket)
				{
					serverConnectedSockets[numConnected] = clientSocket;
					this->numConnected++;
					// TODO: Inform CLIENT about player number
				}
			}
			// TODO: Send some "ready" message to clients
			break;

		default:
			break;
	}
}

bool GameNetworking::isLocal() {return this->role == LOCAL;}
bool GameNetworking::isClient() {return this->role == CLIENT;}
bool GameNetworking::isServer() {return this->role == SERVER;}
int GameNetworking::getPlayers() {return this->players;}
NetRole GameNetworking::getRole() {return this->role;}
