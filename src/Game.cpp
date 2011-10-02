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
	this->screen = screen;

	for (int i = 0; i < 5; i++) player[i] = 0;

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

	currentPlayer = 1;
}

Game::~Game() {
}

int Game::start() {
	draw();

	while (1) {
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			int x, y;
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x / 60;
				y = event.button.y / 60;
				int end;
				if (end = move(x, y)) {
					currentPlayer = currentPlayer % 4 + 1;
					while (!player[currentPlayer] && !player[0])
						currentPlayer = currentPlayer % 4 + 1;

					draw();
				}
				if (end == 2) {
					return 2;
				}
				break;

			case SDL_QUIT:
				return 0;
				break;
			}
		}
	}

	return 0;
}

void Game::draw() {
	SDL_Rect *rect = new SDL_Rect();
	rect->h = rect->w = 600;
	rect->x = rect->y = 0;
	SDL_FillRect(screen, rect, colors[currentPlayer + 4]);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			rect = new SDL_Rect();
			rect->h = rect->w = 60 / 10 * field[i][j];
			rect->x = 60*i + (10-field[i][j]) * 4;
			rect->y = 60*j + (10-field[i][j]) * 4;

			SDL_FillRect(screen, rect, colors[owner[i][j]]);
			delete rect;
		}
	}
	SDL_UpdateRect(screen, 0, 0, 600, 600);
}

int Game::move(int x, int y) {
	if (owner[x][y] == currentPlayer || owner[x][y] == 0) {
		field[x][y]++;
		if (owner[x][y] != currentPlayer) {
			player[currentPlayer]++;
			player[owner[x][y]]--;
			owner[x][y] = currentPlayer;
		}

		draw();
		SDL_Delay((100));

		if (player[currentPlayer] == 64) {
			cout << "Player " << currentPlayer << " won!";
			return 2;
		}

		while (field[x][y] > surrounding[x][y]) {
			field[x][y] -= surrounding[x][y];
			roll(x-1, y);
			roll(x+1, y);
			roll(x, y-1);
			roll(x, y+1);
		}

	}
	else {
		return 0;
	}

	if (player[currentPlayer] == 64) {
		cout << "Player " << currentPlayer << " won!";
		return 2;
	}

	return 1;
}

int Game::roll(int x, int y) {
	if (x < 0 || x > 9 || y < 0 || y > 9) {
		return 0;
	}

	field[x][y]++;
	if (owner[x][y] != currentPlayer) {
		player[currentPlayer]++;
		player[owner[x][y]]--;
		owner[x][y] = currentPlayer;
	}

	draw();
	SDL_Delay((100));

	if (player[currentPlayer] == 64) {
		return 2;
	}

	while (field[x][y] > surrounding[x][y]) {
		field[x][y] -= surrounding[x][y];
		roll(x-1, y);
		roll(x+1, y);
		roll(x, y-1);
		roll(x, y+1);
	}
	return 1;
}
