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

QtRenderer::QtRenderer() {
	game = 0;
	display = new JCCQWidget(this);
}

QtRenderer::~QtRenderer() {
	delete game;
	game = 0;
}

int QtRenderer::newGame(int players, int fieldsize) {
	// Discard old game
	delete game;
	// Start new one
	game = new Game(players, fieldsize, (Renderer*) this);
	return 1;
}

void QtRenderer::closeGame() {
	delete game;
	game = 0;
}

void QtRenderer::push(Map m) {
	// Get map from Game and append it to map list
	maps.push_back(m);
	// Start displaying of maps
	if (display) display->startAnimation();
}

void QtRenderer::flush() {
	// Remove all but one maps from map list
	while (maps.size() > 1) {
		maps.pop_front();
	}

	// Start displaing of maps
	if (display) display->startAnimation();
}

bool QtRenderer::listEmpty() {
	// Check whether list is empty or not
	return maps.empty();
}

Map QtRenderer::update() {
	if (maps.size() <= 0) {
		// No map to deliver? Get actual map from Game
		if (game) {
			return game->getActualMap();
		}
		// No game instance? Draw empty map
		// TODO: Draw something beautiful
		Map m;
		return m;
	}
	
	// Take first map out of list and deliver it
	Map map = maps.front();
	maps.pop_front();
	return map;
}

vector<int> QtRenderer::getStats() {
	if (game) {
		return game->getStats();
	}
	vector<int> a;
	return a;
}

int QtRenderer::mapEvent(int x, int y) {
	// Pass move event to Game
	if (game) return game->move(x, y);
	return 0;
}
