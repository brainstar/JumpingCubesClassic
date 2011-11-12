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

void QtRenderer::startGame(int players) {
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

void QtRenderer::push(Field f) {
	if (running) {
		fields.push_back(f);
		display->startTimer();
	}
}

void QtRenderer::flush() {
	if (!display) {
		return;
	}
	
	while (fields.size() > 1) {
		fields.pop_front();
	}
	
	Field a = fields.front();
	display->update(a);
	fields.pop_front();
}

int QtRenderer::update() {
	if (fields.size() > 0) {
		Field a = fields.front();
		display->update(a);
		fields.pop_front();
	}
	if (fields.size() > 0) {
		return 0; // More fields to draw
	}
	return 1; // No more fields to draw
}

void QtRenderer::gameOver(int winner) {
	running = false;
	//TODO: Display something great
}

int QtRenderer::mouseEvent(float x, float y) {
	if (running) {
		return game->move(x, y);
	}
	return -1;
}
