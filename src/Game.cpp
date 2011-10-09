/*
 * Game.cpp
 *
 *  Created on: 02.10.2011
 *      Author: brainstar
 */

#include "Game.h"
#include <iostream>
#include <SDL/SDL_video.h>

Game::Game() {
}

Game::Game(SDL_Surface *screen) {
	// Initialize the whole thing
	init(screen);
}

Game::~Game() {
}

void Game::init(SDL_Surface *screen) {
	// Set SDL_Surface to draw on
	this->screen = screen;
	scrLength = screen->h;
	fieldLength = scrLength / 10;

	// Create colors
	SDL_PixelFormat *fmt = this->screen->format;
	black = SDL_MapRGB(fmt, 0, 0, 0);
	semiblack = SDL_MapRGBA(fmt, 0, 0, 0, 127);
	colors[0] = SDL_MapRGB(fmt, 127, 127, 127);
	colors[1] = SDL_MapRGB(fmt, 255, 0, 0);
	colors[2] = SDL_MapRGB(fmt, 0, 255, 0);
	colors[3] = SDL_MapRGB(fmt, 0, 0, 255);
	colors[4] = SDL_MapRGB(fmt, 255, 255, 0);
	colors[5] = SDL_MapRGB(fmt, 20, 0, 0);
	colors[6] = SDL_MapRGB(fmt, 0, 20, 0);
	colors[7] = SDL_MapRGB(fmt, 0, 0, 20);
	colors[8] = SDL_MapRGB(fmt, 50, 30, 0);

	// Initialize count of players
	players = 0;

	// Initialize the field
	reset();
}

void Game::reset() {
	// Reset all players with to 0 fields
	for (int i = 0; i < 5; i++) player[i] = 0;

	// Transfer ownership to no one (== player 0) and reset field values
	for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				field[i][j] = 1;
				owner[i][j] = 0;
				surrounding[i][j] = 4;
				if (0 == i % 9) surrounding[i][j]--;
				if (0 == j % 9) surrounding[i][j]--;
				player[0]++;
			}
	}

	// Reset to first player
	currentPlayer = 1;
}

// 0-player game
int Game::startRandom() {
		// No players, filling field with random moves (4 "players")
		players = 4;
		int x, y;

		// Main loop
		while (1) {
			// Generate random move
			do {
				x = rand() % 10;
				y = rand() % 10;
			} while (!move(x, y)); // If illegal move, try again
			SDL_Delay(200);

			// Next player
			next();

			// Redraw field to change background color to next player
			draw();

			// If someone won, end the game
			if (over()) {
				return 1;
			}

			// User chooses to exit the game
			SDL_Event event;
			if (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					return 0;
					break;
				}
			}
		}
}


// Local game
int Game::startLocal() {
	// Main loop
	while (1) {
		// Catching mouse events and SDL_QUIT event
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			// Player clicked on a field
			case SDL_MOUSEBUTTONDOWN:
				// Calculate indices of chosen field
				int x, y;
				x = event.button.x / fieldLength;
				y = event.button.y / fieldLength;

				// If legal move
				if (move(x, y)) {
					// Move on to next player
					next();

					// Redraw field to change background color to next player
					draw();
				}

				// Someone won?
				if (int winner = over()) {
					cout << "Player " << winner << "just WON the game!" << endl << "It's okay! You're free!" << endl;
					return 1;
				}
				break;

			// Player clicked on exit
			case SDL_QUIT:
				return 0;
				break;
			}
		}
	}
}

int Game::start() {
	// Draw gamefield at beginning
	draw();

	// 0-player mode
	if (players == 0) {
		while (startRandom()) {
			reset();
		}
		return 0;
	}

	// (1-4)-player mode
	return startLocal();
}

int Game::setPlayers(int i) {
	// Range check
	if (i < 0 || i > 4) {
		return 0;
	}
	// Set amount of players
	players = i;

	return 1;
}

void Game::draw() {
	// Fill background color with color of active player
	SDL_Rect *rect = new SDL_Rect();
	rect->h = rect->w = scrLength;
	rect->x = rect->y = 0;
	SDL_FillRect(screen, rect, colors[currentPlayer + 4]);

	// Draw the fields
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			rect = new SDL_Rect();

			// Height and Width according to field value
			rect->h = rect->w = (scrLength / 100) * field[i][j];
			rect->x = fieldLength * i + ((fieldLength - rect->w) / 2);
			rect->y = fieldLength * j + ((fieldLength - rect->h) / 2);

			SDL_FillRect(screen, rect, colors[owner[i][j]]);
			delete rect;
		}
	}

	// Put the whole thing to the screen
	SDL_UpdateRect(screen, 0, 0, scrLength, scrLength);
}

int Game::move(int x, int y) {
	// Range check of input values
	if (x < 0 || x > 9 || y < 0 || y > 9) {
		return 0;
	}

	// If legal move
	if (owner[x][y] == currentPlayer || owner[x][y] == 0) {
		// Increase field value
		field[x][y]++;

		// Update field ownership
		if (owner[x][y] != currentPlayer) {
			player[currentPlayer]++;
			player[owner[x][y]]--;
			owner[x][y] = currentPlayer;
		}

		// Redraw and delay to make chain reactions visible
		draw();
		SDL_Delay((100));

		// Someone already won? No need to resume calculation of chain reaction
		if (over()) {
			return 2;
		}

		// Calculating chain reaction
		while (field[x][y] > surrounding[x][y]) {
			field[x][y] -= surrounding[x][y];

			// Someone won? No need to resum calculation of chain reaction
			if (roll(x-1, y) == 2)
				return 2;
			if (roll(x+1, y) == 2)
				return 2;
			if (roll(x, y-1) == 2)
				return 2;
			if (roll(x, y+1) == 2)
				return 2;
		}

		// Move calculated
		return 1;
	}
	// Illegal move
	else {
		return 0;
	}
}

int Game::roll(int x, int y) {
	// Range check of input values
	if (x < 0 || x > 9 || y < 0 || y > 9) {
		return 0;
	}

	// Increase field value
	field[x][y]++;

	// Update field ownership
	if (owner[x][y] != currentPlayer) {
		player[currentPlayer]++;
		player[owner[x][y]]--;
		owner[x][y] = currentPlayer;
	}

	// Redraw and delay to make chain reactions visible
	draw();
	SDL_Delay((100));

	// Someone already won? No need to resume calculation of chain reaction
	if (over()) {
		return 2;
	}

	// Calculating chain reaction
	while (field[x][y] > surrounding[x][y]) {
		field[x][y] -= surrounding[x][y];

		// Someone won? No need to resum calculation of chain reaction
		if (roll(x-1, y) == 2)
			return 2;
		if (roll(x+1, y) == 2)
			return 2;
		if (roll(x, y-1) == 2)
			return 2;
		if (roll(x, y+1) == 2)
			return 2;
	}

	// Move calculated
	return 1;
}


int Game::over() {
	// Someone just won the game?
	for (int i = 0; i < players; i++) {
		if (player[i] == 100) {
			// Yey! Return the glory winner
			return i;
		}
	}

	// Nope.
	return 0;
}

void Game::next() {
	// Move on to next player
	currentPlayer = currentPlayer % players + 1;
	while (!player[currentPlayer] && !player[0])
		currentPlayer = currentPlayer % players + 1;
}
