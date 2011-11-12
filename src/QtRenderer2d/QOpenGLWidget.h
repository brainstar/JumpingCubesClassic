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
#include "../JCCTypes.h"
using namespace std;

class QOpenGLWidget : public QGLWidget {
Q_OBJECT;

public:
	QOpenGLWidget(QWidget *parent = 0, char *name = 0);
	~QOpenGLWidget();
	
public slots:
	int setFieldSize(int a);
	void draw(Field f);
	void draw();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);

signals:
	void mouseClicked(float x, float y);
	
private:
	float colors[5][3];
	float black[3];
	int fields;
	Field field;
};

#endif
