// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// HistoMatch.h - Histogram Match widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef HISTOMATCH_H
#define HISTOMATCH_H

#include "ImageFilter.h"

class HistoMatch : public ImageFilter {
	Q_OBJECT

public:
	HistoMatch			(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool, ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
	void		histoMatch	(ImagePtr, ImagePtr, ImagePtr);
	void		initShader();
	void		gpuProgram(int pass);	// use GPU program to apply filter

protected slots:
	void		changeExp	(int);

private:
	// widgets
	QSlider*	m_slider ;	// HistoMatch slider
	QSpinBox*	m_spinBox;	// HistoMatch spin box
	QGroupBox*	m_ctrlGrp;	// groupbox for panel

	// lookup table
	ImagePtr	 m_lut;		// target histogram

	// function to init lookup table
	bool		initLut		(ImagePtr, ImagePtr, int);
};

#endif	// HISTOMATCH_H
