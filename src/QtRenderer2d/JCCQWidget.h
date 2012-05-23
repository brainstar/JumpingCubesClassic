/*
 * JCCQtWidget.h
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#ifndef _JCC_JCCQWIDGET_H_
#define _JCC_JCCQWIDGET_H_

#include <QWidget.h>
#include <QTimer>
#include <QStatusBar>
#include "QOpenGLWidget.h"
#include "QtRenderer.h"
using namespace std;

class JCCQWidget : QWidget {
Q_OBJECT;

public:
	JCCQWidget(QtRenderer* r = 0);
	~JCCQWidget();
	
	void setRenderer(QtRenderer* r);
	void startAnimation();
	void gameOver();
	
public slots:
	void mouseClicked(int x, int y);
	void timerTick();
	void newGame();
	void closeGame();
	
protected: 

signals:
	void signalDraw(Map f);
	void signalDraw();
	
private:
	QOpenGLWidget* gl;
	QStatusBar* status;
	QtRenderer* renderer;
	QTimer* timer;
	int winner;
};

#endif
