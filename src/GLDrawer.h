/*
 * GLDrawer.h
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#ifndef GL_DRAWER_H
#define GL_DRAWER_H

#include <QtOpenGL/qgl.h>
#include <QMouseEvent>
#include "document.h"
using namespace std;

class GLDrawer : public QGLWidget {
Q_OBJECT
public:
	GLDrawer(QWidget *parent = 0, char *name = 0);
	~GLDrawer();
	int setFields(int amount);
	int getFields();
	void setField(int* value, int* owner);

signals:
	void mouseClicked(int x, int y);
	void mouseReleased();
	void draw();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	
private:
	int fields;
	int* fldValue;
	int* fldOwner;
	float colors[5][3];
	float black[3];
	
	int getFldValue(int x, int y);
	int getFldOwner(int x, int y);
};

#endif
