// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Clip.cpp - Clip widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Clip.h"
#include "hw1/HW_clip.cpp"

extern MainWindow *g_mainWindowP;

// uniform ID
enum { THR1, THR2, SAMPLER };
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Clip::Clip:
//
// Constructor.
//
Clip::Clip(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Clip::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Clip::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Clip");

	// layout for assembling filter widget
	QGridLayout *layout = new QGridLayout;

	// alloc array of labels
	QLabel *label[2];

	// create sliders and spinboxes
	for(int i=0; i<2; i++) {
		// create label[i]
		label[i] = new QLabel(m_ctrlGrp);
		if(!i) label[i]->setText("Thr1");
		else   label[i]->setText("Thr2");

		// create slider and spinbox
		m_slider [i] = new QSlider(Qt::Horizontal, m_ctrlGrp);
		m_spinBox[i] = new QSpinBox(m_ctrlGrp);

		// init slider range and value
		m_slider [i]->setRange(0, MaxGray);
		m_slider [i]->setValue(i* MaxGray);

		// init spinbox range and value
		m_spinBox[i]->setRange(0, MaxGray);
		m_spinBox[i]->setValue(i* MaxGray);

		// assemble dialog
		layout->addWidget(label    [i], i, 0);
		layout->addWidget(m_slider [i], i, 1);
		layout->addWidget(m_spinBox[i], i, 2);
	}

	// init signal/slot connections for Clip
	connect(m_slider [0], SIGNAL(valueChanged(int)),    this, SLOT(changeThr1(int)));
	connect(m_spinBox[0], SIGNAL(valueChanged(int)),    this, SLOT(changeThr1(int)));
	connect(m_slider [1], SIGNAL(valueChanged(int)),    this, SLOT(changeThr2(int)));
	connect(m_spinBox[1], SIGNAL(valueChanged(int)),    this, SLOT(changeThr2(int)));

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Clip::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Clip::applyFilter(ImagePtr I1, bool gpuFlag, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// collect parameters
	int thr1 = m_slider[0]->value();
	int thr2 = m_slider[1]->value();

        // apply filter
	if(!(gpuFlag && m_shaderFlag))
		// apply CPU based filter
		clip(I1, thr1, thr2, I2);
	else
		g_mainWindowP->glw()->applyFilterGPU(m_nPasses);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Clip::clip:
//
// Clip intensities of image I1. Output is in I2.
// If    input<t1: output = t1;
// If t1<input<t2: output = input;
// If      val>t2: output = t2;
//
void
Clip::clip(ImagePtr I1, int t1, int t2, ImagePtr I2)
{
	HW_clip(I1, t1, t2, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Clip::changeThr1:
//
// Slot to process change in thr1 caused by moving the slider
//
void
Clip::changeThr1(int val)
{
	int thr2 = m_slider[1]->value();
	if(val > thr2) val = thr2;

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
// Clip::changeThr2:
//
// Slot to process change in thr2 caused by moving the slider
//
void
Clip::changeThr2(int val)
{
	int thr1 = m_slider[0]->value();
	if(val < thr1) val = thr1;

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
// Clip::reset:
//
// Reset parameters.
//
void
Clip::reset()
{
	for(int i=0; i<2; i++) {
		m_slider [i]->blockSignals(true);
		m_slider [i]->setValue(i*MaxGray);
		m_slider [i]->blockSignals(false);

		m_spinBox[i]->blockSignals(true);
		m_spinBox[i]->setValue(i*MaxGray);
		m_spinBox[i]->blockSignals(false);
	}

	// apply filter and display output
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Clip::initShader:
//
// init shader program and parameters.
//
void
Clip::initShader() 
{
	m_nPasses = 1;
	// initialize GL function resolution for current context
	initializeGLFunctions();

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_Thr1"   ] = THR1;
	uniforms["u_Thr2"   ] = THR2;
	uniforms["u_Sampler"] = SAMPLER;

        QString v_name = ":/vshader_passthrough";
        QString f_name = ":/hw1/fshader_clip";
        
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
// Clip::gpuProgram:
//
// Active gpu program
//
void
Clip::gpuProgram(int pass) 
{
	glUseProgram(m_program[pass].programId());
	glUniform1f (m_uniform[pass][THR1], (GLfloat) m_slider[0]->value()/MXGRAY);
	glUniform1f (m_uniform[pass][THR2], (GLfloat) m_slider[1]->value()/MXGRAY);
	glUniform1i (m_uniform[pass][SAMPLER], 0);
}
