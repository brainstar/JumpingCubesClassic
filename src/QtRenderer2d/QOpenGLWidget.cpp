/*
 * QOpenGlWidget.cpp
 *
 *  Created on: 19.10.2011
 *      Author: Christian M.
 */

#include <QColor>
#include <QPainter>
#include <QImage>
#include "QOpenGLWidget.h"
using namespace std;

QOpenGLWidget::QOpenGLWidget(QWidget *parent, char *name) : QGLWidget(parent) {
	QImage* temp = new QImage("../misc/cubes.png");
	for (int i = 0; i < 5; i++) {
		vector<QImage> row;
		for (int j = 0; j < 7; j++) {
			row.push_back(temp->copy(j*33, i*33, 33, 33));
		}
		cubes.push_back(row);
	}
}

QOpenGLWidget::~QOpenGLWidget() {
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
	float wx = gridx / 10. * 8;
	float wy = gridy / 10. * 8;

	QColor col;
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map.size(); j++) {
			f = &(map[i][j]);

			float v = (float) f->value;

 			// Calculate corners of field
			float x1, y1, dx, dy;
			// lower corner = grid width * position
			// center = lower corner + half grid width
			// corner = center - half field width
			// corner = grid width * position + half grid width - half field width
			if (f->mark){
				x1 = gridx * i;
				y1 = gridy * j;
				dx = gridx;
				dy = gridy;
			}
			else {
				x1 = (gridx * i) + (gridx / 2) - (wx / 2);
				y1 = (gridy * j) + (gridy / 2) - (wy / 2);
				dx = wx;
				dy = wy;
			}
			QRect r(x1, y1, dx, dy);

			// Draw cubes :D
			if (v <= 6) {
				painter.drawImage(r, cubes[f->owner][v-1]);
			}
			else {
				painter.drawImage(r, cubes[f->owner][6]);
				painter.drawText(r, QString::number(v));
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
