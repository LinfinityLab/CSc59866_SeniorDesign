// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW0c.h - Header file for HW0c class
//
// Written by: George Wolberg, 2015
// ======================================================================

#ifndef HW0C_H
#define HW0C_H

#include "HW.h"
#include <QGLShaderProgram>
#include <QtOpenGL>

// ----------------------------------------------------------------------
// standard include files
//

class HW0c : public HW {
	Q_OBJECT
public:
	HW0c		(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters
	void		initVertexBuffer();		// init vertices
	void		initShaders	();		// init shaders

public slots:
	void		changeTheta(int);		// respond to rotation slider
	void		reverseOn();			// respond to color reversal (on)
	void		reverseOff();			// respond to color reversal (off)

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene

	void		mousePressEvent	 (QMouseEvent *);
	void		mouseMoveEvent	 (QMouseEvent *);
	void		mouseReleaseEvent(QMouseEvent *);

private:
	int		  m_winW;			// window width
	int		  m_winH;			// window height
	QGLShaderProgram  m_program;			// GLSL program
	bool		  m_reverseColor;		// reverse color flag
	QSlider		 *m_sliderTheta;		// rotation slider
	QSpinBox	 *m_spinBoxTheta;		// rotation spinbox
	QRadioButton	 *m_radio[2];			// radio button for reversing color
	std::vector<vec2> m_points;			// vector of 2D points (XY)
	std::vector<vec3> m_colors;			// vector of 3D colors (RGB)
	int		  m_numPoints;			// number of 2D points
	QMatrix4x4	  m_ModelMatrix;		// 4x4 transformation matrix
	GLuint		  m_u_ModelMatrix;		// shader index to transformation matrix
	GLuint		  m_u_reverseColor;		// shader index to color reversal flag
	QPoint		  m_mousePosition;		// latest mouse position
};

#endif // HW0C_H
