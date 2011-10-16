/*
 * Network.h
 *
 *  Created on: 11.10.2011
 *      Author: brainstar, blacktremolo
 */

#include <SDL/SDL_net.h>

/*
  Super-evil workaround to make mutual inclusion possible
*/
#ifndef GAME_H_
#include "Game.h"
#endif

#ifndef NETWORK_H_
#define NETWORK_H_

enum NetRole {
	LOCAL,
	CLIENT,
	SERVER
};

// TODO: Maybe split server and client related stuff into subclasses of GameNetworking
class GameNetworking
{

public:
	GameNetworking(NetRole role);
	bool isLocal();
	bool isClient();
	bool isServer();
	int getPlayers();
	NetRole getRole();
	

private:
	NetRole role;
	void initLib();
	IPaddress ipaddress;
	int players;

	// ### Server only ###
	TCPsocket serverSocket;
	TCPsocket serverConnectedSockets[4];
	int numConnected; // Amount of connected clients

	// ### Client only ###
	TCPsocket clientConnectedSocket;


};

#endif /* NETWORK_H_ */
























// Fiber cables and broadband networking coming in v2
