/*
 * Game.cpp
 *
 *  Created on: 02.10.2011
 *      Author: Christian M.
 */

#include "Game.h"
#include <iostream>

Game::Game(unsigned int players, unsigned int fieldSize, Renderer* r) {
	renderer = r;
	reset(players, fieldSize);
}

Game::~Game() {
}

Renderer* Game::setRenderer(Renderer* r) {
	renderer = r;
	return renderer;
}

int Game::move(int x, int y) {
	// Range check
	if (x < 0 || x >= map.size() || y < 0 || y >= map.size()) {
		return 0;
	}

	// GameOver check
	if (int winner = over()) {
		return -winner;
	}

	// Permission check
	Field *f = &(map[x][y]);
	if (f->owner && f->owner != currentPlayer) {
		return 0;
	}
	
	// Increase value and push output
	f->value++;
	changeOwner(f, currentPlayer);
	if (renderer) renderer->push(map);
	
	// Set starting conditions for roll
	rollMap[x][y] = true;
	bool bRoll = true;
	
	// Roll
	while (bRoll) {
		// Not roll again by default unless some field actually rolls
		bRoll = false;
		
		// Create new bool field with same size as map
		vector<vector<bool> > newRoll = vector<vector<bool> >(map.size(), vector<bool>(map.size(), false));
		
		// Check every field about +1
		for (int i = 0; i < map.size(); i++) {
			for (int j = 0; j < map.size(); j++) {
				// This field got +1?
				if (rollMap[i][j]) {
					// Get a pointer to this field
					f = &(map[i][j]);

					// This field rolls?
					if (f->value > f->n) {
						// Have another iteration, maybe neighbours will roll
						bRoll = true;
					
						// Subtract field value
						f->value -= f->n;
						
						// Left neighbour does exist?
						if (i > 0) {
							// Get iterator
							Field *x = &(map[i-1][j]);
							// +1
							x->value++;
							// New owner
							changeOwner(x, currentPlayer);
							// Check for roll in next iteration
							newRoll[i-1][j] = true;
						}
						// Right neighbour does exist?
						if (i < map.size() - 1) {
							Field *x = &(map[i+1][j]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i+1][j] = true;
						}
						// Upper neighbour does exist?
						if (j > 0) {
							Field *x = &(map[i][j-1]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i][j-1] = true;
						}
						// Lower neighbour does exist?
						if (j < map.size() - 1) {
							Field *x = &(map[i][j+1]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i][j+1] = true;
						}
					}
				}
			}
		}
		
		// Update roll information
		rollMap = newRoll;
		
		// Push the new field to the renderer
		if (renderer) renderer->push(map);
		
		// Game over?
		if (over()) {
			// No further iteration
			bRoll = false;
		}
	}
	
	// Game over? Promote winner!
	if (int winner = over()) {
		return -winner;
	}
	else {
		// Choose who's next
		next();
	}
	
	// Return new player
	return currentPlayer;
}

Map Game::getActualMap() {
	return map;
}

int Game::reset(int p, int s) {
	// Instantiate empty Field f for multiple purposes
	Field f;
	
	// Set amount of players
	players = p;
	player.resize(players + 1);
	
	// Update map size
	map.resize(s);
	maxcount = s * s;
	
	// Resize roll map
	rollMap = vector<vector<bool> >(map.size(),
		vector<bool>(map.size(), false));
	
	// Fill map with data
	for (int i = 0; i < map.size(); i++) {
		vector<Field> row = vector<Field>(map.size());
		for (int j = 0; j < map.size(); j++) {
			Field g;
			g.x = i;
			g.y = j;
			g.owner = 0;
			g.value = 1;
			
			g.n = 4;
			if (0 == i % (map.size() - 1)) {
				g.n--;
			}
			if (0 == j % (map.size() - 1)) {
				g.n--;
			}
			
			row[j] = g;
		}
		map[i] = row;
	}
	
	// Fill field statistics with data
	player[0] = maxcount;
	for (int i = 1; i <= players; i++) {
		player[i] = 0;
	}
	currentPlayer = 1;
	
	// If renderer is available, push the map to the renderer
	if (renderer) {
		renderer->push(map);
		renderer->flush();
	}

	return 1;
}

void Game::changeOwner(Field *f, int p) {
	if (p > 0 && p <= players) {
		player[f->owner]--;
		f->owner = p;
		player[f->owner]++;
	}
}

int Game::over()
{
	// Someone just won the game?
	for (int i = 0; i < players; i++)
	{
		if (player[i] == maxcount)
		{
			// Yey! Return the glory winner
			return i;
		}
	}

	// Nope.
	return 0;
}

void Game::next()
{
	// Move on to next player 
	do {
		currentPlayer = (currentPlayer % players) + 1;
	}
	while (!(player[currentPlayer]) && !(player[0]));
}
