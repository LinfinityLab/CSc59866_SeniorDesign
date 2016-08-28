// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW0a.cpp - HW0a class
//
// Written by: George Wolberg, 2015
// ===============================================================

#include "HW0a.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::HW0a:
//
// HW0a constructor.
//
HW0a::HW0a(QWidget *parent)
	 : HW (parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW0a::initializeGL()
{
	// init state variables
	glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f   (1.0, 1.0, 1.0);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW0a::resizeGL(int w, int h)
{
	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// init viewing coordinates for orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::paintGL:
//
// Update GL scene.
//
void
HW0a::paintGL()
{
	// clear canvas with background values
	glClear(GL_COLOR_BUFFER_BIT);

	// define polygon
	glBegin(GL_POLYGON);
		glVertex2f(0.25, 0.25);
		glVertex2f(0.75, 0.25);
		glVertex2f(0.75, 0.75);
		glVertex2f(0.25, 0.75);
	glEnd();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW0a::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 0a");

	return(groupBox);
}
