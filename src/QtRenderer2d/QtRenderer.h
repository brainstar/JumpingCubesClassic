/*
 * QtRenderer.h
 *
 *  Created on: 30.10.2011
 *      Author: Christian M.
 */
 
#ifndef _JCC_QTRENDERER_H_
#define _JCC_QTRENDERER_H_

#include <list>
#include "../Renderer.h"
#include "../Game.h"

class QtRenderer : public Renderer {
public:
	QtRenderer();
	virtual ~QtRenderer();

	// Display access
	int newGame(int players = 2, int fieldsize = 5);
	void closeGame();
	int mapEvent(int x, int y);
	Map update();
	
	// Game access 
	void push(Map m);
	void flush();

	bool listEmpty();
	
private:
	Game *game;
	class JCCQWidget *display;
	list<Map> maps;
};

#endif
