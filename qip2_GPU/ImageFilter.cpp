// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2016 by George Wolberg
//
// ImageFilter.cpp - ImageFilter class. Base class of homework solutions.
//
// Written by: George Wolberg, 2016
// ===============================================================

#include "ImageFilter.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::HW:
//
// HW constructor.
// This is base class for homework solutions that will replace
// the control panel, reset function, and add homework solution. 
//
ImageFilter::ImageFilter(QWidget *parent)
	: QWidget(parent), m_shaderFlag(false)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImageFilter::controlPanel:
//
// Create a control panel of widgets for homework solution.
//
QGroupBox*
ImageFilter::controlPanel()
{
	return NULL;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImageFilter::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Return 1 for success, 0 for failure.
//
bool
ImageFilter::applyFilter(ImagePtr, bool, ImagePtr) 
{
	return true;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImageFilter::reset:
//
// Reset parameters in control panel.
//
void
ImageFilter::reset() {}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImageFilter::initShader:
//
// init shader program and parameters.
//
void
ImageFilter::initShader() {}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ImageFilter::gpuProgram:
//
// Active gpu program
//
void
ImageFilter::gpuProgram(int) {}
