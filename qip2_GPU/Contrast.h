// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.h - Contrast widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef CONTRAST_H
#define CONTRAST_H

#include "ImageFilter.h"

class Contrast : public ImageFilter {
	Q_OBJECT

public:
	Contrast			(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool, ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
	void		contrast	(ImagePtr, double, double, ImagePtr);
	void		initShader();
	void		gpuProgram(int pass);	// use GPU program to apply filter

protected slots:
	void changeBrightnessI(int);
	void changeContrastI  (int);

	void changeBrightnessD(double);
	void changeContrastD  (double);

private:
	// widgets
	QSlider*	m_slider [2];	// brightness/contrast sliders
	QDoubleSpinBox*	m_spinBox[2];	// brightness/contrast spin boxes
	QGroupBox*	m_ctrlGrp;	// groupbox for panel
};

#endif	// CONTRAST_H
