// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Threshold.cpp - Threshold class
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Threshold.h"
#include "hw1/HW_threshold.cpp"

extern MainWindow *g_mainWindowP;


// uniform ID
enum { THR, SAMPLER };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::Threshold:
//
// Constructor.
//
Threshold::Threshold(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Threshold::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Threshold");

	// create label
	QLabel *label = new QLabel("Thr");

	// create slider
	m_slider  = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_slider->setRange(1, MXGRAY);
	m_slider->setValue(MXGRAY>>1);
	m_slider->setTickPosition(QSlider::TicksBelow);
	m_slider->setTickInterval(25);

	// create spinbox
	m_spinBox = new QSpinBox(m_ctrlGrp);
	m_spinBox->setRange(1, MXGRAY);
	m_spinBox->setValue(MXGRAY>>1);

	// init signal/slot connections for Threshold
	connect(m_slider , SIGNAL(valueChanged(int)), this, SLOT(changeThr(int)));
	connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeThr(int)));

	// assemble dialog
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(  label  , 0, 0);
	layout->addWidget(m_slider , 0, 1);
	layout->addWidget(m_spinBox, 0, 2);

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Threshold::applyFilter(ImagePtr I1, bool gpuFlag, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// get threshold value
	int thr = m_slider->value();

	// error checking
	if(thr < 0 || thr > MXGRAY) return 0;

	if(!(gpuFlag && m_shaderFlag))
		// apply CPU based filter
		threshold(I1, thr, I2);
	else 
		g_mainWindowP->glw()->applyFilterGPU(m_nPasses);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::threshold:
//
// Threshold I1 using threshold thr. Output is in I2.
// val<thr: 0;	 val >= thr: MaxGray (255)
//
void
Threshold::threshold(ImagePtr I1, int thr, ImagePtr I2)
{
	HW_threshold(I1, thr, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::changeThr:
//
// Slot to process change in thr caused by moving the slider.
//
void
Threshold::changeThr(int thr)
{
	// set slider and spinbox to same value
	m_slider ->blockSignals(true);
	m_slider ->setValue    (thr );
	m_slider ->blockSignals(false);
	m_spinBox->blockSignals(true);
	m_spinBox->setValue    (thr );
	m_spinBox->blockSignals(false);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::reset:
//
// Reset parameters.
//
void
Threshold::reset()
{
	// reset parameters
	int thr = MXGRAY>>1;
	m_slider ->blockSignals(true);
	m_slider ->setValue    (thr);
	m_slider ->blockSignals(false);
	m_spinBox->blockSignals(true);
	m_spinBox->setValue    (thr);
	m_spinBox->blockSignals(false);

	// apply filter and display output
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::initShader:
//
// init shader program and parameters.
//
void
Threshold::initShader() 
{
	m_nPasses = 1;
	// initialize GL function resolution for current context
	initializeGLFunctions();

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_thr"    ] = THR;
	uniforms["u_Sampler"] = SAMPLER;
	
        QString v_name = ":/vshader_passthrough";
        QString f_name = ":/hw1/fshader_threshold";
        
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
// Threshold::gpuProgram:
//
// Active Threshold gpu program
//
void
Threshold::gpuProgram(int pass)
{
	glUseProgram(m_program[pass].programId());
	glUniform1f (m_uniform[pass][THR], (GLfloat) m_slider->value()/MXGRAY);
	glUniform1i (m_uniform[pass][SAMPLER], 0);
}
