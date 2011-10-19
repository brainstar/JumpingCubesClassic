/*
 * GLDrawer.cpp
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#include "gl_drawer.h"
using namespace std;

GLDrawer::GLDrawer(QWidget *parent, char *name) : QGLWidget(parent) {
}

GLDrawer::~GLDrawer() {
}

int GLDrawer::setFields(int amount) {
	if (amount > 0 && amount <= 20) {
		fields = amount;
	}
	return fields;
}

int GLDrawer::getFields() {
	return fields;
}

void GLDrawer::setField(int* value, int* owner) {
	if (value != NULL && owner != NULL) {
		fldValue = value;
		fldOwner = owner;
	}
}

void GLDrawer::initializeGL() {
	// Initializing OpenGL
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_MAP1_VERTEX_3);
}

void GLDrawer::resizeGL(int w, int h) {
	// Resize the Window to display at least what is determined by the grid
	h = h ? h : 1;
	w = w ? w : 1;

	glViewport(0, 0, h, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0., 10., 0., 10., 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLDrawer::paintGL() {
	// Draw everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	for (int i = 0; i < fields; i++) {
		for (int j = 0; j < fields; j++) {
			int v = getFldValue(i, j);
 			float w = (1.0 / fields) * v;
			float x1, y1, x2, y2;
			x1 = 10.0 / fields * i + (5.0 / fields - w / 2.);
			y1 = 10.0 / fields * j + (5.0 / fields - w / 2.);
			x2 = x1 + w;
			y2 = y1 + w;
			
			glColor3fv(colors[getFldOwner(i, j)]);
			
			glBegin(GL_QUADS);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);
			glEnd();
		}
	}
	// TODO: Render everything
}

void GLDrawer::mousePressEvent(QMouseEvent * event) {
	// Event processing of a mouse click	
	if (event->button() & Qt::LeftButton) {
		int x, y;
		x = event->x() * fields / size().width();
		y = event->y() * fields / size().height();
		
		emit mouseClicked(x, y);
	}

	QGLWidget::mousePressEvent(event);
}

int GLDrawer::getFldValue(int x, int y) {
	return *(field + (x * fields) + y);
}

int GLDrawer::getFldOwner(int x, int y) {
	return *(owner + (x * fields) + y);
}
