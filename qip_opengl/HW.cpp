// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW.cpp - HW class. Base class of homework solutions.
//
// Written by: George Wolberg, 2015
// ===============================================================

#include "HW.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::HW:
//
// HW constructor.
// This is base class for homework solutions that will replace
// the control panel, reset function, and add homework solution. 
//
HW::HW(QWidget *parent)
   : QGLWidget (parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::controlPanel:
//
// Create a control panel of widgets for homework solution.
//
QGroupBox*
HW::controlPanel()
{
	return NULL;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::reset:
//
// Reset parameters in control panel.
//
void
HW::reset() {}
