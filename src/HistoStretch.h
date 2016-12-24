// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// HistoStretch.h - Histogram Stretch widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef HISTOSTRETCH_H
#define HISTOSTRETCH_H

#include "ImageFilter.h"

class HistoStretch : public ImageFilter {
	Q_OBJECT

public:
	HistoStretch			(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool, ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
	void		histoStretch	(ImagePtr, int, int, ImagePtr);
	void		initShader();
	void		gpuProgram(int pass);	// use GPU program to apply filter

protected slots:
	void		changeThr1	(int);
	void		changeThr2	(int);
	void		setThrFlag1	(int);
	void		setThrFlag2	(int);

private:
	// widgets
	QSlider*	m_slider [2];	// HistoStretch sliders
	QSpinBox*	m_spinBox[2];	// HistoStretch spin boxes
	QCheckBox*	m_thr1;		// min histogram threshold control
	QCheckBox*	m_thr2;		// max histogram threshold control
	QGroupBox*	m_ctrlGrp;	// groupbox for panel

	// state variables
	int		 m_thrFlag1;	// flag for finding min histogram value
	int		 m_thrFlag2;	// flag for finding max histogram value
};

#endif	// HISTOSTRETCH_H
