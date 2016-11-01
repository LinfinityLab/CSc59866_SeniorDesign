// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Sharpen.h - Sharpen widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef SHARPEN_H
#define SHARPEN_H

#include "ImageFilter.h"

class Sharpen : public ImageFilter {
	Q_OBJECT

public:
	Sharpen				(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool, ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
	void		sharpen		(ImagePtr, int, double, ImagePtr);
	void		initShader();
	void		gpuProgram(int pass);	// use GPU program to apply filter

protected slots:
	void		changeSize	(int);
	void		changeFactor	(int);

private:
	// widgets
	QSlider*	m_slider [2];	// Blur sliders
	QSpinBox*	m_spinBox[2];	// Blur spin boxes
	QGroupBox*	m_ctrlGrp;	// groupbox for panel
	int		m_width;	// input image width
	int		m_height;	// input image height
};

#endif	// SHARPEN_H
