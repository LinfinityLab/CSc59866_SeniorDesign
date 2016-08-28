// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW.h - Header file for HW class. Base class of homework solutions.
//
// Written by: George Wolberg, 2015
// ======================================================================

#ifndef HW_H
#define HW_H

#include <QtWidgets>
#include <QGLWidget>
#include <OpenGL/glu.h>
#include <QGLFunctions>

typedef QVector2D vec2;
typedef QVector3D vec3;

enum {
	ATTRIB_VERTEX,
	ATTRIB_COLOR,
	ATTRIB_TEXTURE_POSITION
};

// ----------------------------------------------------------------------
// standard include files
//
class HW : public QGLWidget, protected QGLFunctions {

public:
	HW(QWidget *parent = 0);
	virtual QGroupBox*	controlPanel();		// create control panel
	virtual void		reset();		// reset parameters
};

#endif // HW_H
