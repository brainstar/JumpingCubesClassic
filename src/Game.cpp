/*
 * Game.cpp
 *
 *  Created on: 02.10.2011
 *      Author: Christian M.
 */

#include "Game.h"
#include <iostream>

Game::Game() {
	renderer = 0;
	reset(2, 5);
}

Game::~Game() {
}

int Game::setRenderer(Renderer* r) {
	if (!r) {
		return 0;
	}
	
	renderer = r;
	return 1;
}

int Game::startGame(int players, int fieldSize, Renderer* r) {
	if (!r) {
		return 0;
	}
	
	setRenderer(r);

	reset(players, fieldSize);
	running = true;

	return 1;
}

void Game::reset() {
	reset(players, map.size);
}

void Game::reset(int p, int s) {
	// Stop Game
	running = false;
	// Instantiate empty Field f for multiple purposes
	Field f;

	// Map size and player count range check
	if (s < 4 || s > 20) {
		return;
	}
	if (p < 1 || p > 4) {
		return;
	}
	
	// Set amount of players
	players = p;
	
	// Update map size
	map.size = s;
	maxcount = s * s;
	// Resize map
	map.m.resize(map.size, vector<Field>(map.size, f));
	// Resize roll map
	rollMap = vector<vector<bool> >(map.size, vector<bool>(map.size, false));
	
	// Fill map with data
	for (int i = 0; i < map.size; i++) {
		vector<Field> row = vector<Field>(map.size);
		for (int j = 0; j < map.size; j++) {
			Field g;
			g.x = i;
			g.y = j;
			g.owner = 0;
			g.value = 1;
			
			g.n = 4;
			if (0 == i % (map.size - 1)) {
				g.n--;
			}
			if (0 == j % (map.size - 1)) {
				g.n--;
			}
			
			row[j] = g;
		}
		map.m[i] = row;
	}
	
	// Fill field statistics with data
	player[0] = maxcount;
	for (int i = 1; i < 5; i++) {
		player[i] = 0;
	}
	currentPlayer = 1;
	
	// If renderer is available, push the map to the renderer
	if (renderer) {
		renderer->push(map);
		renderer->flush();
	}
}

int Game::move(float x, float y) {
	// Range check
	if (x < 0. || x >= 1. || y < 0. || y >= 1.) {
		return 0;
	}
	
	// Transform [0.0, 1.0[ into [0, size[
	int ix = (int) (x * map.size);
	int iy = (int) (y * map.size);
	
	// Permission check
	Field *f = &(map.m[ix][iy]);
	if (f->owner && f->owner != currentPlayer) {
		return 0;
	}
	
	// Increase value and push output
	f->value++;
	changeOwner(f, currentPlayer);
	renderer->push(map);
	
	// Set starting conditions for roll
	rollMap[ix][iy] = true;
	bool bRoll = true;
	
	// Roll
	while (bRoll) {
		bRoll = false;
		vector<vector<bool> > newRoll = vector<vector<bool> >(map.size, vector<bool>(map.size, false));
		for (int i = 0; i < map.size; i++) {
			for (int j = 0; j < map.size; j++) {
				if (rollMap[i][j]) {
					f = &(map.m[i][j]);

					if (f->value > f->n) {
						bRoll = true;
						f->value -= f->n;
						if (i > 0) {
							Field *x = &(map.m[i-1][j]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i-1][j] = true;
						}
						if (i < map.size - 1) {
							Field *x = &(map.m[i+1][j]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i+1][j] = true;
						}
						if (j > 0) {
							Field *x = &(map.m[i][j-1]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i][j-1] = true;
						}
						if (j < map.size - 1) {
							Field *x = &(map.m[i][j+1]);
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
		renderer->push(map);
		
		// Over
		if (over()) {
			bRoll = false;
		}
	}
	
	if (int winner = over()) {
		renderer->gameOver(winner);
		running = false;
		cout << "Wheee!!!" << endl;
	}
	else {
		next();
	}
	
	return currentPlayer;
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
