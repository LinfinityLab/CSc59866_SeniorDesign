// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.h - Threshold widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "ImageFilter.h"


class Threshold : public ImageFilter {
	Q_OBJECT

public:
	Threshold			(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool, ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
	void		threshold	(ImagePtr, int, ImagePtr);
	void		initShader	();
	void		gpuProgram	(int pass);			// use GPU program to apply filter

	


protected slots:
	void		changeThr	(int);

private:
	// widgets
	QSlider*	m_slider ;	// Threshold slider
	QSpinBox*	m_spinBox;	// Threshold spinbox
	QGroupBox*	m_ctrlGrp;	// Groupbox for panel

};

#endif	// THRESHOLD_H
