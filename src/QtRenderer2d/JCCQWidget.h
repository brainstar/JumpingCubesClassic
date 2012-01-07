/*
 * JCCQtWidget.h
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#ifndef _JCC_JCCQWIDGET_H_
#define _JCC_JCCQWIDGET_H_

#include <Qt/qwidget.h>
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
	void gameOver(int winner);
	
public slots:
	void mouseClicked(float x, float y);
	void timerTick();
	void newGame();
	
protected: 

signals:
	void signalDraw(Map f);
	void signalDraw();
	
private:
	QOpenGLWidget* gl;
	QStatusBar* status;
	QtRenderer* renderer;
	QTimer* timer;
};

#endif
