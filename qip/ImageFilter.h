// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2016 by George Wolberg
//
// ImageFilter.h - Header file for ImageFilter class. Base class of homework solutions.
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <QtWidgets>
#include "IP.h"
using namespace IP;

// ----------------------------------------------------------------------
// standard include files
//
class ImageFilter : public QWidget {

public:
	ImageFilter(QWidget *parent = 0);
	virtual QGroupBox* controlPanel	();		// create control panel
	virtual bool	   applyFilter	(ImagePtr, ImagePtr); // filter input image -> make output
	virtual void	   reset	();		// reset parameters
};

#endif // IMAGEFILTER_H
