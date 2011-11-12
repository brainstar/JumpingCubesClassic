/*
 * QOpenGlWidget.cpp
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#include "QOpenGLWidget.h"
using namespace std;

QOpenGLWidget::QOpenGLWidget(QWidget *parent, char *name) : QGLWidget(parent) {
	for (int i = 0; i < 3; i++) {
		colors[0][i] = 0.5f;
	}
	colors[1][0] = 1.0f;
	colors[1][1] = colors[1][2] = 0.0f;
	
	colors[2][1] = 1.0f;
	colors[2][0] = colors[2][2] = 0.0f;
	
	colors[3][2] = 1.0f;
	colors[3][0] = colors[3][1] = 0.0f;
	
	colors[4][0] = colors[4][1] = 1.0f;
	colors[4][2] = 0.0f;
}

QOpenGLWidget::~QOpenGLWidget() {
}

int QOpenGLWidget::setFieldSize(int a) {
	if (fields == a || a < 3 || a > 20) {
		return fields;
	}
	fields = a;
	return fields;
}

void QOpenGLWidget::draw(Field f) {
	field = f;
	draw();
}

void QOpenGLWidget::draw() {
	paintGL();
}

void QOpenGLWidget::initializeGL() {
	// Initializing OpenGL
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_MAP1_VERTEX_3);
}

void QOpenGLWidget::resizeGL(int w, int h) {
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

void QOpenGLWidget::paintGL() {
	// Draw everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	Element *e;
	for (int i = 0; i < fields; i++) {
		for (int j = 0; j < fields; j++) {
			e = &(field[i][j]);
			
			int v = e->value;
 			float w = (1.0 / fields) * v;
 			
			float x1, y1, x2, y2;
			x1 = 10.0 / fields * i + (5.0 / fields - w / 2.);
			y1 = 10.0 / fields * j + (5.0 / fields - w / 2.);
			x2 = x1 + w;
			y2 = y1 + w;
			
			glColor3fv(colors[e->owner]);
			
			glBegin(GL_QUADS);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);
			glEnd();
			glBegin(GL_POINTS);
				glVertex2f(0.0f, 0.0f);
				glVertex2f(1.0f, 0.0f);
				glVertex2f(1.0f, 1.0f);
			glEnd();
		}
	}
}

void QOpenGLWidget::mousePressEvent(QMouseEvent * event) {
	// Event processing of a mouse click	
	if (event->button() & Qt::LeftButton) {
		float x, y;
		x = (float) event->x() / size().width();
		y = (float) event->y() / size().height();
		
		emit mouseClicked(x, y);
	}

	QGLWidget::mousePressEvent(event);
}
