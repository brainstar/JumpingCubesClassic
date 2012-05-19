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

	renderer = r;
	this->resize(500, 500);
	
	timer = new QTimer();
	timer->setInterval(100);
	
	QMenuBar *menubar = new QMenuBar();
	QAction *action;
	QMenu *menu;
	
	menu = menubar->addMenu("Game");
	action = menu->addAction("New Game");
	connect(action, SIGNAL(triggered()), this, SLOT(newGame()));
	action = menu->addAction("Close Game");
	connect(action, SIGNAL(triggered()), this, SLOT(closeGame()));
	
	gl = new QOpenGLWidget();
	status = new QStatusBar();
	
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(menubar, 0);
	layout->addWidget(gl, 1);
	layout->addWidget(status, 0);
	this->setLayout(layout);
	
	connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
	connect(gl, SIGNAL(mouseClicked(int, int)), this, SLOT(mouseClicked(int, int)));
	connect(this, SIGNAL(signalDraw(Map)), gl, SLOT(slotDraw(Map)));
	connect(this, SIGNAL(signalDraw()), gl, SLOT(slotDraw()));

	gl->hide();
	this->show();
}

JCCQWidget::~JCCQWidget() {
}

void JCCQWidget::setRenderer(QtRenderer* r) {
	renderer = r;
}

void JCCQWidget::startAnimation() {
	if (!(timer->isActive())) {
		timer->start();
	}
}

void JCCQWidget::gameOver() {
	QString msg = "Player ";
	msg += winner;
	msg += " won!";
	status->showMessage(msg, 0);
}

void JCCQWidget::mouseClicked(int x, int y) {
	if (!renderer) {
		return;
	}

	if (winner) {
		return;
	}
	
	int player = renderer->mapEvent(x, y);
	if (player == -10) {
		status->showMessage("Invalid target", 2000);
	}
	else if (player < 0) {
		winner = -player;
		gameOver();
	}
	else if (player == 0) {
		status->showMessage("Invalid move", 2000);
	}
	else if (player > 0) {
		QString msg = "Next player: ";
		msg += player;
		status->showMessage(msg, 0);
	}
}

void JCCQWidget::timerTick() {
	if (!renderer) {
		return;
	}

	if (renderer->listEmpty()) {
		timer->stop();
	}
	else {
		emit signalDraw(renderer->update());
	}
}

void JCCQWidget::newGame() {
	if (!renderer) {
		return;
	}
	if (!(renderer->newGame(2, 5))) {
		status->showMessage("Error starting game", 2000);
		return;
	}

	winner = 0;
}

void JCCQWidget::closeGame() {
	if (!renderer) {
		return;
	}
	if (!(renderer->newGame(0, 0))) {
		status->showMessage("Error closing game", 2000);
		return;
	}

	winner = -1;
}
