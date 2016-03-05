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
	Threshold	(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset		();		// reset parameters

protected:
	void threshold(ImagePtr I1, int thr, ImagePtr I2);

protected slots:
	void changeThr(int);

private:
	// threshold controls
	QSlider		*m_slider ;	// Threshold sliders
	QSpinBox	*m_spinBox;	// Threshold spin boxes

	// label for Otsu thresholds
	QLabel		*m_label;	// Label for printing Otsu thresholds

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};


#endif	// THRESHOLD_H
