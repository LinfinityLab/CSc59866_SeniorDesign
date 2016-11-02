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
#include "GLWidget.h"
#include "IP.h"
using namespace IP;

enum { PASS1, PASS2 };
// ----------------------------------------------------------------------
// standard include files
//
class ImageFilter : public QWidget, protected QGLFunctions {

public:
	ImageFilter(QWidget *parent = 0);
	virtual QGroupBox* controlPanel	();			    // create control panel
	virtual bool	   applyFilter	(ImagePtr, bool, ImagePtr); // filter input image -> make output
	virtual void	   reset	();			    // reset parameters
	virtual void	   initShader   ();
	virtual void	   gpuProgram   (int pass);		    // use GPU program to apply filter
	bool		   gpuImplemented() { return m_shaderFlag;}
	int		   gpuPasses     () { return m_nPasses;}

protected:
	bool			m_shaderFlag;				// true if the shaders are initialized
	QGLShaderProgram	m_program[MXPROGRAMS];			// GLSL programs
	GLint			m_uniform[MXPROGRAMS][MXUNIFORMS];	// uniform vars for each program
	int			m_nPasses;				// number of GPU passes
};

#endif // IMAGEFILTER_H
