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
	grey = SDL_MapRGB(fmt, 127, 127, 127);
	red = SDL_MapRGB(fmt, 255, 0, 0);
	green = SDL_MapRGB(fmt, 0, 255, 0);
	blue = SDL_MapRGB(fmt, 0, 0, 255);
	yellow = SDL_MapRGB(fmt, 255, 255, 0);

	currentPlayer = 1;
}

Game::~Game() {
	SDL_Quit();
}

void Game::start() {
	draw();

	while (1) {
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			int x, y;
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x / 60;
				y = event.button.y / 60;
				if (move(x, y)) {
					currentPlayer = currentPlayer % 4 + 1;
					while (!player[currentPlayer] && !player[0])
						currentPlayer = currentPlayer % 4 + 1;
				}
				break;

			case SDL_QUIT:
				delete this;
				break;
			}
		}
	}
}

void Game::draw() {
	SDL_Rect *rect = new SDL_Rect();
	rect->h = rect->w = 600;
	rect->x = rect->y = 0;
	SDL_FillRect(screen, rect, black);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			rect = new SDL_Rect();
			rect->h = rect->w = 60 / 10 * field[i][j];
			rect->x = 60*i + (10-field[i][j]) * 4;
			rect->y = 60*j + (10-field[i][j]) * 4;

			unsigned int color;
			switch (owner[i][j]) {
			case 0:
				color = grey;
				break;
			case 1:
				color = red;
				break;
			case 2:
				color = green;
				break;
			case 3:
				color = blue;
				break;
			case 4:
				color = yellow;
				break;
			default:
				color = black;
				break;
			}

			SDL_FillRect(screen, rect, color);
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
			exit(1);
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

	while (field[x][y] > surrounding[x][y]) {
		field[x][y] -= surrounding[x][y];
		roll(x-1, y);
		roll(x+1, y);
		roll(x, y-1);
		roll(x, y+1);
	}
	return 1;
}
