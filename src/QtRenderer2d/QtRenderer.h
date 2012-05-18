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
	~QtRenderer();
	
	Game* setGame(Game *g);

	// Display access
	void newGame(int players = 0, int fieldsize = 0);
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
