// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Dummy.cpp - Dummy control panel
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Dummy.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::Dummy:
//
// Constructor.
//
Dummy::Dummy(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::createPanel:
//
// Create group box for control panel.
//
QGroupBox*
Dummy::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("");
	return(m_ctrlGrp);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::initShader:
//
// init shader program and parameters.
//
void
Dummy::initShader() {}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::gpuProgram:
//
// Active gpu program
//
void
Dummy::gpuProgram(int) {}
