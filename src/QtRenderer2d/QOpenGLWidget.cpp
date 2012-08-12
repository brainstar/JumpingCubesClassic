/*
 * QOpenGlWidget.cpp
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#include <QColor>
#include <QPainter>
#include "QOpenGLWidget.h"
using namespace std;

QOpenGLWidget::QOpenGLWidget(QWidget *parent, char *name) : QGLWidget(parent) {
	createColors(10);
}

QOpenGLWidget::~QOpenGLWidget() {
}

void QOpenGLWidget::createColors(int n) {
	vector<float> x;
	x.resize(3);
	colors.resize(n + 1, x);

	for (int i = 0; i < 3; i++) {
		colors[0][i] = 0.5f;
	}
	
	for (int i = 1; i <= n; i++) {
		QColor col = QColor::fromHsvF(((float) i - 1) / n, 1.0f, 1.0f);
		int id = (i + 1) / 2 + (i % 2 ? 0 : 5);
		colors[id][0] = col.redF();
		colors[id][1] = col.greenF();
		colors[id][2] = col.blueF();
	}
}

void QOpenGLWidget::slotDraw(Map m) {
	// Get map from main widget
	map = m;
	if (map.size() == 0) {
		// TODO: No map? Display something beautiful
		hide();
	}
	else {
		// Show map
		show();
	}
	// Tell OpenGL to redraw the scene
	slotDraw();
}

void QOpenGLWidget::slotDraw() {
	// Tell OpenGL to redraw the Scene
	repaint();
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
	// Resize window
	h = h ? h : 1;
	w = w ? w : 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0., 10., 10., 0., 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void QOpenGLWidget::paintEvent(QPaintEvent *event) {
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// TODO: move draw code over here
	if (map.size() <= 0) {
		painter.setPen(QColor::fromRgb(0,0,0));
		painter.drawRect(0, 0, size().width(), size().height());
		painter.end();
		return;
	}

	Field *f;
	float s = (float) map.size();
	float gridx = (float) size().width() / s;
	float gridy = (float) size().height() / s;

	QColor col;
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map.size(); j++) {
			f = &(map[i][j]);

			float v = (float) f->value;
			float wx = gridx / 10. * v;
			float wy = gridy / 10. * v;


 			// Calculate corners of field
			float x1, y1;
			// lower corner = grid width * position
			// center = lower corner + half grid width
			// corner = center - half field width
			// corner = grid width * position + half grid width - half field width
			x1 = (gridx * i) + (gridx / 2) - (wx / 2);
			y1 = (gridy * j) + (gridy / 2) - (wy / 2);

			col = QColor::fromRgbF(colors[f->owner][0],
				colors[f->owner][1],
				colors[f->owner][2]);
			painter.setPen(col);
			painter.fillRect(x1, y1, wx, wy, col);
			col.darker();
			painter.setPen(QColor::fromRgb(0,0,0));
			painter.drawText(QRectF(gridx * i,
				gridy * j,
				gridx,
				gridy),
				Qt::AlignCenter,
				QString::number(f->value));
		}
	}
	painter.end();
}
void QOpenGLWidget::paintGL() {
	/*// Draw everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// TODO: Empty map? Draw something beautiful
	if (map.size() <= 0) {
		return;
	}
	
	// Create field pointer, get field size
	Field *f;
	//QPainter painter(this);
	float s = (float) map.size();
	// Iterate through all fields
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map.size(); j++) {
			// Set pointer to field i|j
			f = &(map[i][j]);
			
			// Get value of field and convert it to field width
			float v = (float) f->value;
			// Symbol field = 1/10 grid width * value
 			float w = (1.0 / s) * v;
 			
 			// Calculate corners of field
			float x1, y1, x2, y2;
			// lower corner = grid width * position
			// center = lower corner + half grid width
			// corner = center - half field width
			// corner = grid width * position + (half grid width - half field width)
			x1 = 10.0f / s * i + (5.0f / s - w / 2.f);
			y1 = 10.0f / s * j + (5.0f / s - w / 2.f);
			// upper corner = lower corner + field width
			x2 = x1 + w;
			y2 = y1 + w;
			
			// Pick color
			if (f->owner > 10) {
				glColor3f(0.9f, 0.9f, 0.9f);
			}
			else {
				glColor3fv(&(colors[f->owner][0]));
			}

			// Draw field
			glBegin(GL_QUADS);
				glVertex3f(x1, y1, 0.0f);
				glVertex3f(x2, y1, 0.0f);
				glVertex3f(x2, y2, 0.0f);
				glVertex3f(x1, y2, 0.0f);
			glEnd();

			// Draw value text
			// TODO: Crappy crap doesn't work
			QColor col = QColor::fromRgb(colors[f->owner][0],
				colors[f->owner][1],
				colors[f->owner][2]);
			col.darker();
			painter.setPen(col);
			painter.setFont(QFont());
			painter.drawText(QRectF(size().width() / s * i,
				size().height() / s * j,
				size().width() / s,
				size().height() / s),
				Qt::AlignCenter,
				QString::number(f->value));
		}
	} */
}

void QOpenGLWidget::mousePressEvent(QMouseEvent * event) {
	// Event processing of a mouse click	
	if (event->button() & Qt::LeftButton) {
		// Convert position from pixel to field units
		int x = (int) ( 
			(float) event->x()  /  size().width()  *  map.size() );
		int y = (int) (
			(float) event->y() / size().height() * map.size() );

		// Hand over event to main widget
		emit mouseClicked(x, y);
	}

	// Let Qt handle unused mouse events
	QGLWidget::mousePressEvent(event);
}
