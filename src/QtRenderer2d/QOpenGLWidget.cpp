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
	
	colors[4][2] = 0.0f;
	colors[4][0] = colors[4][1] = 1.0f;
}

QOpenGLWidget::~QOpenGLWidget() {
}

void QOpenGLWidget::slotDraw(Map m) {
	map = m;
	if (map.size() == 0) {
		hide();
	}
	else {
		show();
	}
	slotDraw();
}

void QOpenGLWidget::slotDraw() {
	updateGL();
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

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0., 10., 10., 0., 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void QOpenGLWidget::paintGL() {
	// Draw everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if (map.size() <= 0) {
		return;
	}
	
	Field *f;
	float s = (float) map.size();
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map.size(); j++) {
			f = &(map[i][j]);
			
			float v = (float) f->value;
 			float w = (1.0 / s) * v;
 			
			float x1, y1, x2, y2;
			x1 = 10.0f / s * i + (5.0f / s - w / 2.f);
			y1 = 10.0f / s * j + (5.0f / s - w / 2.f);
			x2 = x1 + w;
			y2 = y1 + w;
			
			glColor3fv(colors[f->owner]);
//			glColor3f(0.0f, 1.0f, 0.0f);
			
//			qDebug("%f|%f - %f|%f - %f|%f - %f|%f", x1, y1, x2, y1, x2, y2, x1, y2);
			glBegin(GL_QUADS);
				glVertex3f(x1, y1, 0.0f);
				glVertex3f(x2, y1, 0.0f);
				glVertex3f(x2, y2, 0.0f);
				glVertex3f(x1, y2, 0.0f);
			glEnd();
		}
	}
}

void QOpenGLWidget::mousePressEvent(QMouseEvent * event) {
	// Event processing of a mouse click	
	if (event->button() & Qt::LeftButton) {
		float x, y;
		x = (float) event->x() / size().width() * map.size();
		y = (float) event->y() / size().height() * map.size();
		
		int ix = (int) x;
		int iy = (int) y;
		emit mouseClicked(ix, iy);
	}

	QGLWidget::mousePressEvent(event);
}
