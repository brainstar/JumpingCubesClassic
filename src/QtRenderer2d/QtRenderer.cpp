/*
 * QtRenderer.cpp
 *
 *  Created on: 30.10.2011
 *      Author: Christian M.
 */

#include "QtRenderer.h"
#include "JCCQWidget.h"

//TODO: Much to do

QtRenderer::QtRenderer() {
	display = new JCCQWidget(this);
	running = false;
}

QtRenderer::~QtRenderer() {
}

void QtRenderer::setGame(Game *g) {
	game = g;
}

void QtRenderer::startGame(int players, int fieldsize) {
	if (!running) {
		running = game->startGame(players, fieldsize, this);
	}
}

void QtRenderer::stopGame() {
	if (running) {
		game->reset();
		running = false;
	}
}

void QtRenderer::toggleGame(bool b) {
	running = b;
}

void QtRenderer::push(Map m) {
	if (running) {
		maps.push_back(m);
		display->startAnimation();
	}
}

void QtRenderer::flush() {
	if (!display) {
		return;
	}
	
	while (maps.size() > 1) {
		maps.pop_front();
	}

	display->startAnimation();
}

Map QtRenderer::update() {
	if (fields.size() > 0) {
		Map a = fields.front();
		fields.pop_front();
		return a;
	}
	
	return 0;
}

void QtRenderer::gameOver(int winner) {
	running = false;
	display->gameOver(winner);
}

int QtRenderer::mouseEvent(float x, float y) {
	if (running) {
		return game->move(x, y);
	}
	return -1;
}
