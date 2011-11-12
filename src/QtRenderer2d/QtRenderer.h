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
	
	void setGame(Game *g);
	void startGame(int players = 0, int fieldsize = 0);
	void stopGame();
	void toggleGame(bool b);
	
	void push(Field f);
	void flush();
	int update();
	void gameOver(int winner);
	
	int mouseEvent(float x, float y);
	
private:
	Game *game;
	bool running;
	class JCCQWidget *display;
	list<Field> fields;
};

#endif
