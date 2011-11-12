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
	if (i < 0 || i > 4)
	{
		return 0;
	}
	// Set amount of players
	players = i;

	return 1;
}

int Game::setFieldSize(int a) {
	if (size == a || a < 3 || a > 20) {
		return size;
	}

	size = a;
	
	Element e;
	e.x = e.y = e.owner = e.value = e.n = 0;
	vector<Element> row = vector<Element>(size, e);
	field.resize(size, row);
	
	roll = vector<vector<bool> >(size, vector<bool>(size, false));

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

	return 1;
}

void Game::reset() {
	running = false;
	
	Element e;
	
	for (int i = 0; i < size; i++) {
		vector<Element> row = vector<Element>(size);
		for (int j = 0; j < size; j++) {
			e.x = i;
			e.y = j;
			e.owner = 0;
			e.value = 1;
			
			e.n = 4;
			if (0 == i % size) {
				e.n--;
			}
			if (0 == j % size) {
				e.n--;
			}
			
			row[j] = e;
		}
		field[i] = row;
	}
	
	player[0] = size * size;
	for (int i = 1; i < 5; i++) {
		player[i] = 0;
	}
	currentPlayer = 1;
	
	if (renderer) {
		renderer->push(field);
		renderer->flush();
	}
}

int Game::move(float x, float y) {
	// Range check
	if (x < 0. || x >= 1. || y < 0. || y >= 1.) {
		return 0;
	}
	// Transform [0.0, 1.0[ itno [0, size[
	int ix = (int) (x * size);
	int iy = (int) (y * size);
	
	// Permission check
	Element *e = &(field[ix][iy]);
	if (e->owner && e->owner != currentPlayer) {
		return 0;
	}
	
	// Increase value and push output
	e->value++;
	changeOwner(e, currentPlayer);
	renderer->push(field);
	
	// Set starting conditions for roll
	roll[ix][iy] = true;
	bool bRoll = true;
	
	// Roll
	while (bRoll) {
		bRoll = false;
		vector<vector<bool> > newRoll = vector<vector<bool> >(size, vector<bool>(size, false));
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (roll[i][j]) {
					e = &(field[i][j]);
					
					if (e->value > e->n) {
						bRoll = true;
						e->value -= e->n;
						if (i > 0) {
							Element *x = &(field[i-1][j]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i-1][j] = true;
						}
						if (i < size - 1) {
							Element *x = &(field[i+1][j]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i+1][j] = true;
						}
						if (j > 0) {
							Element *x = &(field[i][j-1]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i][j-1] = true;
						}
						if (j < size - 1) {
							Element *x = &(field[i][j+1]);
							x->value++;
							changeOwner(x, currentPlayer);
							newRoll[i][j+1] = true;
						}
					}
				}
			}
		}
		
		// Update roll information
		roll = newRoll;
		
		// Push the new field to the renderer
		renderer->push(field);
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

void Game::changeOwner(Element *e, int p) {
	if (p > 0 && p <= players) {
		player[e->owner]--;
		e->owner = p;
		player[e->owner]++;
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
