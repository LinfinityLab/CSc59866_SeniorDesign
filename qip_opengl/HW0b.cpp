// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW0b.cpp - HW0b class
//
// Written by: George Wolberg, 2015
// ===============================================================

#include "HW0b.h"
#include "MainWindow.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::HW0b:
//
// HW0b constructor.
//
HW0b::HW0b(QWidget *parent)
	: HW(parent), m_angle(0.0f)
{
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW0b::initializeGL()
{
	// init state variables
	glClearColor(1.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f   (1.0, 1.0, 1.0);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW0b::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float xmax, ymax;
	if(m_checkBoxAR->isChecked()) {
		float ar = (float) w / h;
		if(ar > 1.0) {		// wide screen
			xmax = ar;
			ymax = 1.;
		} else {		// tall screen
			xmax = 1.;
			ymax = 1/ar;
		}
	} else {
		xmax = 1.0;
		ymax = 1.0;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// init viewing coordinates for orthographic projection
	glLoadIdentity();
	if(m_checkBoxFlip->isChecked())
		glOrtho(-xmax, xmax,  ymax, -ymax, -1.0, 1.0);
	else	glOrtho(-xmax, xmax, -ymax,  ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::paintGL:
//
// Update GL scene.
//
void
HW0b::paintGL()
{
	// initial data
	QVector2D v[] = {
		QVector2D(-.25, -.25),
		QVector2D( .25, -.25),
		QVector2D( .15,  .25),
		QVector2D(-.15,  .25)
	};

	// clear canvas with background values
	glClear(GL_COLOR_BUFFER_BIT);

	// init cosine and sine variables
	double c = cos(m_angle);
	double s = sin(m_angle);

	// define polygon
	glBegin(GL_POLYGON);
		for(int i=0; i<4; i++) {
		    glVertex2f(c*v[i].x() - s*v[i].y(), s*v[i].x() + c*v[i].y()); 
		}
	glEnd();
	glFlush();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW0b::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 0b");
	groupBox->setMinimumWidth(300);

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;

	// create checkboxes
	m_checkBoxFlip = new QCheckBox("Flip y-coordinates");
	m_checkBoxAR   = new QCheckBox("Maintain aspect ratio");
	m_checkBoxFlip->setChecked(false);
	m_checkBoxAR  ->setChecked(true );

	// create slider to rotate data
	m_slider = new QSlider(Qt::Horizontal);
	m_slider->setRange(0, 360);
	m_slider->setValue(0);

	// create spinBox
	m_spinBox = new QSpinBox;
	m_spinBox->setRange(0, 360);
	m_spinBox->setValue(0);

	// slider label to display name
	QLabel *label = new QLabel("Rotation");

	// assemble widgets into layout
	layout->addWidget(m_checkBoxFlip, 0, 0, 1, 3);
	layout->addWidget(m_checkBoxAR  , 1, 0, 1, 3);
	layout->addWidget(label,     2, 0);
	layout->addWidget(m_slider,  2, 1);
	layout->addWidget(m_spinBox, 2, 2);

	// assign layout to group box
	groupBox->setLayout(layout);

	// init signal/slot connections
	connect(m_checkBoxFlip, SIGNAL(stateChanged(int)), this, SLOT(flipY(int)));
	connect(m_checkBoxAR  , SIGNAL(stateChanged(int)), this, SLOT(aspect(int)));
	connect(m_slider,	SIGNAL(valueChanged(int)), this, SLOT(rotate(int)));
	connect(m_spinBox,	SIGNAL(valueChanged(int)), this, SLOT(rotate(int)));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::flipY:
//
// Slot function to flip y-coordinates.
//
void
HW0b::flipY(int state)
{
	// update checkbox
	m_checkBoxFlip->setChecked(state);

	// call resizeGL() to reset coordinate system 
	resizeGL(m_winW, m_winH);

	// redraw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::aspect:
//
// Slot function to maintain aspect ratio.
//
void
HW0b::aspect(int state)
{
	// update checkbox
	m_checkBoxAR->setChecked(state);

	// call resizeGL() to reset coordinate system 
	resizeGL(m_winW, m_winH);

	// redraw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::rotate:
//
// Slot function to rotate data.
//
void
HW0b::rotate(int angle)
{
	// update slider and spinbox
	m_slider->blockSignals(true);
	m_slider->setValue(angle);
	m_slider->blockSignals(false);

	m_spinBox->blockSignals(true);
	m_spinBox->setValue(angle);
	m_spinBox->blockSignals(false);

	// convert angle to radians
	m_angle = angle * (M_PI / 180.);

	// redraw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0b::reset:
//
// Reset parameters.
//
void
HW0b::reset()
{
	// reset checkboxes
	m_checkBoxFlip->setChecked(false);
	m_checkBoxAR  ->setChecked(true );
	resizeGL(m_winW, m_winH);

	// reset angle and slider/spinbox settings
	m_angle = 0;
	m_slider ->setValue(m_angle);
	m_spinBox->setValue(m_angle);

	// redraw
	updateGL();
}
