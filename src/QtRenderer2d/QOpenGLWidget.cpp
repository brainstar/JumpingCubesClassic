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

QColor QOpenGLWidget::getPlayerColor(int n) {
	if (n >= colors.size()) {
		return QColor::fromRgb(0,0,0);
	}
	return QColor::fromRgbF(colors[n][0],
				colors[n][1],
				colors[n][2]);
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

void QOpenGLWidget::paintEvent(QPaintEvent *event) {
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.fillRect(0, 0,
		size().width(), size().height(),
		QColor::fromRgb(0,0,0));

	if (map.size() <= 0) {
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
			if (f->value > 1) {
				painter.setPen(QColor::fromRgb(0,0,0));
				painter.drawText(QRectF(gridx * i, gridy * j, gridx, gridy),
					Qt::AlignCenter,
					QString::number(f->value));
			}
		}
	}
	painter.end();
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
