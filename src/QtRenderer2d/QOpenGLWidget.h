/*
 * QOpenGLWidget.h
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#ifndef _JCC_QOPENGLWIDGET_H_
#define _JCC_QOPENGLWIDGET_H_

#include <QtOpenGL/qgl.h>
#include <QMouseEvent>
#include <QTimer>
#include <QColor>
#include <vector>
#include "../JCCTypes.h"
using namespace std;

class QOpenGLWidget : public QGLWidget {
Q_OBJECT;

public:
	QOpenGLWidget(QWidget *parent = 0, char *name = 0);
	virtual ~QOpenGLWidget();
	QColor getPlayerColor(int i);
	
public slots:
	void slotDraw(Map m);
	void slotDraw();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);

signals:
	void mouseClicked(int x, int y);
	
private:
	vector<vector<float> > colors;
	float black[3];
	Map map;

	void createColors(int n);
};

#endif
