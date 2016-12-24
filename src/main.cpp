// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2016 by George Wolberg
//
// main.c - main() function.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);		// create application
	MainWindow window;			// create UI window
	window.setMinimumSize(QSize(1366, 768));
	window.showMaximized();			// display window
	return app.exec();			// infinite processing loop
}
