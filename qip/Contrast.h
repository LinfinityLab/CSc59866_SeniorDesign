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
	Contrast	(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset		();		// reset parameters

protected:
	void contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2);

protected slots:

private:
	// brightness/contrast controls
	QSlider		*m_sliderB ;	// brightness slider
	QSlider		*m_sliderC ;	// contrast   slider
	QSpinBox	*m_spinBoxB;	// brightness spin box
	QSpinBox	*m_spinBoxC;	// contrast   spin box

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// groupbox for panel
};

#endif	// CONTRAST_H
