/*
 * main.cpp
 *
 *  Created on: 02.10.2011
 *      Author: Christian M.
 */

#include <iostream>
#include <QApplication>
#include "QtRenderer.h"
#include "../Game.h"
#include "main_qt.h"
using namespace std;

int main_qt(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	new QtRenderer();
	
	return app.exec();
}
