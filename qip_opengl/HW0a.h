// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW0a.h - Header file for HW0a class
//
// Written by: George Wolberg, 2015
// ======================================================================

#ifndef HW0A_H
#define HW0A_H

#include "HW.h"

// ----------------------------------------------------------------------
// standard include files
//

class HW0a : public HW {
	Q_OBJECT
public:
	HW0a		(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();		// create control panel

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene
};

#endif // HW0A_H
