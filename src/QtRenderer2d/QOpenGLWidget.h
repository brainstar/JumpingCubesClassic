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
#include "../JCCTypes.h"
using namespace std;

class QOpenGLWidget : public QGLWidget {
Q_OBJECT;

public:
	QOpenGLWidget(QWidget *parent = 0, char *name = 0);
	virtual ~QOpenGLWidget();
	
public slots:
	void slotDraw(Map m);
	void slotDraw();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);

signals:
	void mouseClicked(int x, int y);
	
private:
	float colors[5][3];
	float black[3];
	Map map;
};

#endif
