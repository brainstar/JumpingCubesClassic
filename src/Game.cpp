/*
 * Game.cpp
 *
 *  Created on: 02.10.2011
 *      Author: Christian M.
 */

#include "Game.h"
#include <iostream>

Game::Game()
{
	setPlayers(0);
	setFieldSize(5);
	renderer = 0;
	reset();
}

Game::~Game()
{
}

int Game::setPlayers(int i)
{
	// Range check
	if (i < 1 || i > 4)
	{
		return 0;
	}
	// Set amount of players
	players = i;

	return 1;
}

int Game::setFieldSize(int a) {
	if (map.size == a || a < 3 || a > 20) {
		return map->size;
	}

	map.size = a;

	Field f;
	f.x = f.y = f.owner = f.value = f.n = 0;
	vector<Field> row = vector<Field>(size, f);
	map.m.resize(size, row);

	rollMap = vector<vector<bool> >(size, vector<bool>(size, false));

	return a;
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
	
	setPlayers(players);
	setFieldSize(fieldSize);
	setRenderer(renderer);

	running = true;

	renderer->push(map);
	renderer->flush();

	return 1;
}

void Game::reset() {
	running = false;
	
	Field f;
	
	for (int i = 0; i < size; i++) {
		vector<Field> row = vector<Field>(size);
		for (int j = 0; j < size; j++) {
			f.x = i;
			f.y = j;
			f.owner = 0;
			f.value = 1;
			
			f.n = 4;
			if (0 == i % size) {
				f.n--;
			}
			if (0 == j % size) {
				f.n--;
			}
			
			row[j] = e;
		}
		map.m[i] = row;
	}
	
	player[0] = size * size;
	for (int i = 1; i < 5; i++) {
		player[i] = 0;
	}
	currentPlayer = 1;
	
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
	// Transform [0.0, 1.0[ itno [0, size[
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
		vector<vector<bool> > newRoll = vector<vector<bool> >(size, vector<bool>(size, false));
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
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
						if (i < size - 1) {
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
						if (j < size - 1) {
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

		if (int winner = over()) {
			renderer->gameOver(winner);
			bRoll = false;
			running = false;
		}
		else {
			next();
		}
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
		if (player[i] == 100)
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
		currentPlayer = currentPlayer % players + 1;
	}
	while (!player[currentPlayer] && !player[0]);
}
