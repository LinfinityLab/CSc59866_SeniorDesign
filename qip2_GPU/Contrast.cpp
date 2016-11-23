// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.cpp - Brightness/Contrast widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Contrast.h"
#include "hw1/HW_contrast.cpp"

void Contrast::changeBrightnessD(double val) { changeBrightnessI((int) val); }
void Contrast::changeContrastD  (double val) { changeContrastI  ((int) val); }

extern MainWindow *g_mainWindowP;
enum { BRIGHTNESS, U_CONTRAST, SAMPLER };
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
Contrast::Contrast(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Contrast::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Contrast Enhancement");

	// layout for assembling filter widget
	QGridLayout *layout = new QGridLayout;

	// alloc array of labels
	QLabel *label[2];

	// create sliders and spinboxes
	int max;
	for(int i=0; i<2; i++) {
		// create label[i]
		label[i] = new QLabel(m_ctrlGrp);
		if(!i) label[i]->setText("Brightness");
		else   label[i]->setText("Contrast");

		// init var for range
		if(!i)  max = 256;
		else	max = 100;

		// create slider
		m_slider [i] = new QSlider(Qt::Horizontal, m_ctrlGrp);
		m_slider [i]->setRange(-max, max);
		m_slider [i]->setValue(0);
		m_slider [i]->setTickPosition(QSlider::TicksBelow);
		m_slider [i]->setTickInterval(25);

		// create spinbox
		m_spinBox[i] = new QDoubleSpinBox(m_ctrlGrp);
		m_spinBox[i]->setRange(-max, max);
		m_spinBox[i]->setValue(0);
		m_spinBox[i]->setDecimals(0);

		// assemble dialog
		layout->addWidget(label    [i], i, 0);
		layout->addWidget(m_slider [i], i, 1);
		layout->addWidget(m_spinBox[i], i, 2);
	}

	// init signal/slot connections for Contrast
	connect(m_slider [0], SIGNAL(valueChanged(int)),    this, SLOT(changeBrightnessI(int)));
	connect(m_slider [1], SIGNAL(valueChanged(int)),    this, SLOT(changeContrastI  (int)));
	connect(m_spinBox[0], SIGNAL(valueChanged(double)), this, SLOT(changeBrightnessD(double)));
	connect(m_spinBox[1], SIGNAL(valueChanged(double)), this, SLOT(changeContrastD  (double)));

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Contrast::applyFilter(ImagePtr I1, bool gpuFlag, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// collect parameters
	double b = m_slider[0]->value();	// brightness
	double c = m_slider[1]->value();	// contrast

	// convert contrast from [-100,100] slider range to [0,5] range
	if(c >=0)
		c = c/25.  + 1.0;	// slope: 1 to 5
	else	c = 1 + (c/133.);	// slope: .25 to 1

	// apply filter
	if(!(gpuFlag && m_shaderFlag))
		// apply CPU based filter
		contrast(I1, b, c, I2);
	else
		g_mainWindowP->glw()->applyFilterGPU(m_nPasses);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::contrast:
//
// Apply contrast enhancement to I1. Output is in I2.
// Stretch intensity difference from reference value (128) by multiplying
// difference by "contrast" and adding it back to 128. Shift result by
// adding "brightness" value.
//
void
Contrast::contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2)
{
	HW_contrast(I1, brightness, contrast, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeBrightnessI:
//
// Slot to process change in brightness value caused by moving the slider
// (integer version).
//
void
Contrast::changeBrightnessI(int val)
{
	// set slider and spinbox
	m_slider [0]->blockSignals(true);
	m_slider [0]->setValue    (val );
	m_slider [0]->blockSignals(false);
	m_spinBox[0]->blockSignals(true);
	m_spinBox[0]->setValue    (val );
	m_spinBox[0]->blockSignals(false);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeContrastI:
//
// Slot to process change in contrast value caused by moving the slider
// (integer version).
//
void
Contrast::changeContrastI(int val)
{
	// set slider and spinbox
	m_slider [1]->blockSignals(true);
	m_slider [1]->setValue    (val );
	m_slider [1]->blockSignals(false);
	m_spinBox[1]->blockSignals(true);
	m_spinBox[1]->setValue    (val );
	m_spinBox[1]->blockSignals(false);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Contrast::reset()
{
	for(int i=0; i<2; i++) {
		m_slider [i]->blockSignals(true);
		m_slider [i]->setValue    (0);
		m_slider [i]->blockSignals(false);

		m_spinBox[i]->blockSignals(true);
		m_spinBox[i]->setValue    (0);
		m_spinBox[i]->blockSignals(false);
	}

	// apply filter and display output
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::initShader:
//
// init shader program and parameters.
//
void
Contrast::initShader() 
{
	m_nPasses = 1;
	// initialize GL function resolution for current context
	initializeGLFunctions();

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_Brightness"] = BRIGHTNESS;
	uniforms["u_Contrast"  ] = U_CONTRAST;
	uniforms["u_Sampler"   ] = SAMPLER;

        QString v_name = ":/vshader_passthrough";
        QString f_name = ":/hw1/fshader_contrast";
        
#ifdef __APPLE__
        v_name += "_Mac";
        f_name += "_Mac"; 
#endif    

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	g_mainWindowP->glw()->initShader(m_program[PASS1], 
	                                 v_name + ".glsl", 
	                                 f_name + ".glsl",
					 uniforms,
					 m_uniform[PASS1]);
	m_shaderFlag = true;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::gpuProgram:
//
// Active gpu program
//
void
Contrast::gpuProgram(int pass) 
{
	float c = m_slider[1]->value();	// contrast
	float b = (float) m_slider[0]->value()/MXGRAY;
	// convert contrast from [-100,100] slider range to [0,5] range
	if(c >=0)
		c = c/25.0f  + 1.0f;	// slope: 1 to 5
	else	c = 1 + (c/133.0f);	// slope: .25 to 1
	glUseProgram(m_program[pass].programId());
	glUniform1f (m_uniform[pass][BRIGHTNESS], b);
	glUniform1f (m_uniform[pass][U_CONTRAST], c);
	glUniform1i (m_uniform[pass][SAMPLER], 0);
}