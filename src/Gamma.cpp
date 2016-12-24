// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Gamma.cpp - Gamma correction widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Gamma.h"
#include "hw1/HW_gamma.cpp"

extern MainWindow *g_mainWindowP;
// uniform ID
enum { GAMMA_VALUE, SAMPLER };
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::Gamma:
//
// Constructor.
//
Gamma::Gamma(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Gamma::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Gamma Correction");

	// create label
	QLabel *label = new QLabel("Gamma");

	// create slider
	m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_slider->setRange(1, 100);
	m_slider->setValue(10);
	m_slider->setTickPosition(QSlider::TicksBelow);
	m_slider->setTickInterval(25);

	// create spinbox
	m_spinBox = new QDoubleSpinBox(m_ctrlGrp);
	m_spinBox->setRange(0.1, 10.);
	m_spinBox->setValue	(1.0);
	m_spinBox->setSingleStep(0.1);
	m_spinBox->setDecimals	(2);

	// init signal/slot connections for Gamma
	connect(m_slider,  SIGNAL(valueChanged( int  )), this, SLOT(changeGammaI( int  )));
	connect(m_spinBox, SIGNAL(valueChanged(double)), this, SLOT(changeGammaD(double)));

	// assemble dialog
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label    , 0, 0);
	layout->addWidget(m_slider , 0, 1);
	layout->addWidget(m_spinBox, 0, 2);

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Gamma::applyFilter(ImagePtr I1, bool gpuFlag,  ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// get gamma value
	double gamma = m_slider->value() * 0.1;

	// error checking
	if(gamma < 0.1 || gamma > 10.0) return 0;

        // apply gamma correction
	if(!(gpuFlag && m_shaderFlag)) 
		// apply CPU based filter
		gammaCorrect(I1, gamma, I2);
	 else
		g_mainWindowP->glw()->applyFilterGPU(m_nPasses);
        
	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::gammaCorrect:
//
// Gamma correct image I1. Output is in I2.
//
void
Gamma::gammaCorrect(ImagePtr I1, double gamma, ImagePtr I2)
{
	HW_gammaCorrect(I1, gamma, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::changeGammaI:
//
// Slot to process change in gamma value caused by moving the slider (int)
//
void
Gamma::changeGammaI(int gamma)
{
	// set spinbox
	m_spinBox->setValue(gamma*0.1);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::changeGammaD:
//
// Slot to process change in gamma value caused by moving the spinbox (double)
//
void
Gamma::changeGammaD(double gamma)
{
	// round gamma to the nearest int
	double g = gamma * 10;
	int  val = (g >= 0) ? (int)(g+.5) : -(int)(.5-g);

	// set slider
	m_slider->setValue(val);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::reset:
//
// Reset parameters.
//
void
Gamma::reset()
{
	m_slider ->blockSignals(true);
	m_slider ->setValue    (10);
	m_slider ->blockSignals(false);
	m_spinBox->blockSignals(true);
	m_spinBox->setValue    (10);
	m_spinBox->blockSignals(false);

	// apply filter and display output
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma::initShader:
//
// init shader program and parameters.
//
void
Gamma::initShader() 
{
	m_nPasses = 1;
	// initialize GL function resolution for current context
	initializeGLFunctions();

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_gamma"  ] = GAMMA_VALUE;
	uniforms["u_Sampler"] = SAMPLER;

        QString v_name = ":/vshader_passthrough";
        QString f_name = ":/hw1/fshader_gamma";
        
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
// Gamma::gpuProgram:
//
// Active gpu program
//
void
Gamma::gpuProgram(int pass) 
{
	glUseProgram(m_program[pass].programId());
	glUniform1f (m_uniform[pass][GAMMA_VALUE], (GLfloat) 1.0f/(m_slider->value()*0.1f));
	glUniform1i (m_uniform[pass][SAMPLER], 0);
}