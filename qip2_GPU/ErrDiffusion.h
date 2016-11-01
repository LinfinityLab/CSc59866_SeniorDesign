// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// ErrDuffusion.h - Error Diffusion widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef ERRDIFFUSION_H
#define ERRDIFFUSION_H

#include "ImageFilter.h"


class ErrDiffusion : public ImageFilter {
	Q_OBJECT

public:
	ErrDiffusion			(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool, ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
	void		errDiffusion	(ImagePtr, int, bool, double, ImagePtr);
	void		initShader();
	void		gpuProgram(int pass);	// use GPU program to apply filter

protected slots:
	void		changeMethod	(int);
	void		changeSerpentine(int);
	void		changeGammaI	(int);
	void		changeGammaD	(double);

private:
	// widgets
	QComboBox*	m_comboBox;	// ErrDuffusion comboBox
	QSlider*	m_slider;	// ErrDuffusion slider
	QDoubleSpinBox*	m_spinBox;	// ErrDuffusion spin box
	QCheckBox*	m_checkBox;	// ErrDuffusion checkbox
	QGroupBox*	m_ctrlGrp;	// Groupbox for panel
};

#endif	// ERRDIFFUSION_H
