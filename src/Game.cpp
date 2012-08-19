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

vector<int> Game::getVector(int x, int y) {
	vector<int> v(2);
	v[0] = x;
	v[1] = y;
	return v;
}

void Game::increment(int x, int y) {
	if (x < 0 || x >= map.size() || y < 0 || y >= map.size()) {
		return;
	}
	Field *f = &(map[x][y]);

	f->value++;
	changeOwner(f, currentPlayer);
	iRollMap.push_back(getVector(x, y));
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
	increment(x, y);
	f->mark = true;
	if (renderer) renderer->push(map);
	iRollMap.push_back(getVector(x, y));
	list<vector<int> >::iterator iter;

	while (iRollMap.size()) {
		iter = iRollMap.begin();
		int tx = (*iter)[0], ty = (*iter)[1];
		if (tx >= 0 && tx < map.size() && ty >= 0 && ty < map.size()) {
			f = &(map[tx][ty]);
			if (f->value > f->n) {
				if (!(f->mark)){
					f->mark = true;
					if (renderer) renderer->push(map);
				}
				increment(tx - 1, ty);
				increment(tx + 1, ty);
				increment(tx, ty - 1);
				increment(tx, ty + 1);
				if (renderer) renderer->push(map);
				f->value -= f->n;
			}
			f->mark = false;
			if (renderer) renderer->push(map);
		}
		iRollMap.pop_front();
		
		if (over()) {
			while (iRollMap.size()) {
				iRollMap.pop_front();
			}
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

vector<int> Game::getStats() {
	return player;
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
	for (int i = 0; i <= players; i++)
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
