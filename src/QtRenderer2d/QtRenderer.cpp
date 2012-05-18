/*
 * QtRenderer.cpp
 *
 *  Created on: 30.10.2011
 *      Author: Christian M.
 */

#include "QtRenderer.h"
#include "JCCQWidget.h"
#include <iostream>
using namespace std;

//TODO: Much to do

QtRenderer::QtRenderer() {
	display = NULL;
	game = NULL;
	display = new JCCQWidget(this);
}

QtRenderer::~QtRenderer() {
}

Game* QtRenderer::setGame(Game *g) {
	if (g) game = g;
	
	return g;
}

void QtRenderer::newGame(int players, int fieldsize) {
	if (game) game->newGame(players, fieldsize, this);
}

void QtRenderer::push(Map m) {
	maps.push_back(m);
	if (display) display->startAnimation();
}

void QtRenderer::flush() {
	while (maps.size() > 1) {
		maps.pop_front();
	}
	
	if (display) display->startAnimation();
}

bool QtRenderer::listEmpty() {
	if (maps.size())  {
		return false;
	}
	return true;
}

Map QtRenderer::update() {
	if (maps.size() <= 0) {
		// TODO: In strong need of a "Get me an empty map"-helper-method
	}
	
	Map map = maps.front();
	maps.pop_front();
	return map;
}

int QtRenderer::mapEvent(int x, int y) {
	return game->move(x, y);
}
