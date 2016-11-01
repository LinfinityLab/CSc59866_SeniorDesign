// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Gamma.h - Gamma widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef GAMMA_H
#define GAMMA_H

#include "ImageFilter.h"

class Gamma : public ImageFilter {
	Q_OBJECT

public:
	Gamma				(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool,  ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
	void		gammaCorrect	(ImagePtr, double, ImagePtr);
	void		initShader();
	void		gpuProgram(int pass);	// use GPU program to apply filter

protected slots:
	void changeGammaI( int  );
	void changeGammaD(double);

private:
	// widgets
	QSlider*	m_slider;	// gamma slider
	QDoubleSpinBox*	m_spinBox;	// gamma spin box
	QGroupBox*	m_ctrlGrp;	// groupbox for panel
};

#endif	// GAMMA_H
