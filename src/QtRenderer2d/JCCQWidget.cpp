/*
 * JCCQtWidget.cpp
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#include <QMenuBar>
#include <QVBoxLayout>
#include <iostream>
#include "JCCQWidget.h"
using namespace std;

JCCQWidget::JCCQWidget(QtRenderer* r) {
	winner = 0;

	// Set renderer
	renderer = r;

	// Resize to 500x500
	this->resize(500, 500);
	
	// Set up timer for roll updates
	timer = new QTimer();
	timer->setInterval(1000);
	
	// Create menu bar
	QMenuBar *menubar = new QMenuBar();
	QAction *action;
	QMenu *menu;
	
	// Add entries to menu bar and connect them
	menu = menubar->addMenu("Game");
	action = menu->addAction("New game");
	connect(action, SIGNAL(triggered()), this, SLOT(newGame()));
	action = menu->addAction("Close game");
	connect(action, SIGNAL(triggered()), this, SLOT(closeGame()));
	
	// Create OpenGL widget and status bar
	gl = new QOpenGLWidget();
	status = new QStatusBar();
	
	// Create layout, append all widgets and set layout
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(menubar, 0);
	layout->addStretch(0);
	layout->addWidget(gl, 1);
	layout->addWidget(status, 0);
	this->setLayout(layout);
	
	// Connect timer and gl widget
	connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
	connect(gl, SIGNAL(mouseClicked(int, int)), this, SLOT(mouseClicked(int, int)));
	connect(this, SIGNAL(signalDraw(Map)), gl, SLOT(slotDraw(Map)));
	connect(this, SIGNAL(signalDraw()), gl, SLOT(slotDraw()));

	// Hide gl widget and show main widget
	// TODO: Show something beautiful
	gl->hide();
	this->show();
}

JCCQWidget::~JCCQWidget() {
	// Delete renderer
	delete renderer;
}

QtRenderer* JCCQWidget::setRenderer(QtRenderer* r) {
	// Set renderer;
	renderer = r;
	return renderer;
}

void JCCQWidget::startAnimation() {
	// Start update timer
	if (!(timer->isActive())) {
		timerTick();
		timer->start();
	}
}

void JCCQWidget::gameOver() {
	// Show winner message
	// TODO: Winner number is displayed as special character
	QString msg = "Player ";
	msg += QString::number(winner);
	msg += " won!";
	status->showMessage(msg, 0);
}

void JCCQWidget::mouseClicked(int x, int y) {
	// No one to pass over event
	if (!renderer) {
		return;
	}

	// No need to pass over event
	if (winner) {
		return;
	}
	
	// Pass over event to Renderer
	int player = renderer->mapEvent(x, y);
	// Negative number := -winner
	if (player < 0) {
		winner = -player;
		gameOver();
	}
	// 0 := Invalid move
	else if (player == 0) {
		status->showMessage("Invalid move", 2000);
	}
	// Positive number := ID of next player
	else if (player > 0) {
		QString msg = "Next player: ";
		msg += QString::number(player);
		status->showMessage(msg, 0);
	}
}

void JCCQWidget::timerTick() {
	// No renderer, no one to get maps from
	if (!renderer) {
		timer->stop();
		return;
	}

	// Get new map to draw
	emit signalDraw(renderer->update());
	// Stop timer if everything is done
	if (renderer->listEmpty()) {
		timer->stop();
	}
}

void JCCQWidget::newGame() {
	// No renderer, no new game -> error message
	if (!renderer) {
		status->showMessage("Error starting game: No Renderer available");
		return;
	}
	// Error creating new Game? -> error message
	if (!(renderer->newGame(4, 5))) {
		status->showMessage("Error starting game");
		return;
	}

	// Unset winner to 0
	winner = 0;

	// Show message
	status->showMessage("New game started");
}

void JCCQWidget::closeGame() {
	// No renderer, no game to close
	if (!renderer) {
		return;
	}
	
	// Close game
	renderer->closeGame();

	// Unset winner to -1
	winner = -1;

	// Show message
	status->showMessage("Game closed.");
}
