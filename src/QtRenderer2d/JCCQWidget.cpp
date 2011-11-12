/*
 * JCCQtWidget.cpp
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#include <QMenuBar>
#include <QVBoxLayout>
#include "JCCQWidget.h"
using namespace std;

JCCQWidget::JCCQWidget(QtRenderer* r) {
	renderer = r;
	this->resize(500, 500);
	
	timer = new QTimer();
	timer->setInterval(200);
	
	QMenuBar *menubar = new QMenuBar();
	QAction *action;
	QMenu *menu;
	
	menu = menubar->addMenu("Game");
	action = menu->addAction("New");
	connect(action, SIGNAL(triggered()), this, SLOT(newGame()));
	
	gl = new QOpenGLWidget();
	status = new QStatusBar();
	
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(menubar, 0);
	layout->addWidget(gl, 1);
	layout->addWidget(status, 0);
	this->setLayout(layout);
	
	connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
	connect(gl, SIGNAL(mouseClicked(float, float)), this, SLOT(mouseClicked(float, float)));
	connect(this, SIGNAL(setFieldSize(int)), gl, SLOT(setFieldSize(int)));
	connect(this, SIGNAL(draw(Field)), gl, SLOT(draw(Field)));
	connect(this, SIGNAL(draw()), gl, SLOT(draw()));
	
	gl->show();
	this->show();
}

JCCQWidget::~JCCQWidget() {
}

void JCCQWidget::setRenderer(QtRenderer* r) {
	renderer = r;
}

void JCCQWidget::startTimer() {
	timer->start();
}

void JCCQWidget::update(Field f) {
	emit draw(f);
}

void JCCQWidget::update() {
	emit draw();
}

void JCCQWidget::setFieldSize(int f) {
	gl->setFieldSize(f);
}

void JCCQWidget::mouseClicked(float x, float y) {
	if (!renderer) {
		return;
	}
	
	int player = renderer->mouseEvent(x, y);
	if (player == 0) {
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
	if (renderer->update()) {
		timer->stop();
	}
}

void JCCQWidget::newGame() {
	if (!renderer) {
		return;
	}
	renderer->startGame();
}