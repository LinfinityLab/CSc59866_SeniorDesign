// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Blur.cpp - Blur widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Blur.h"
#include "ops_neighborhood/HW_blur.cpp"

extern MainWindow *g_mainWindowP;
enum { WSIZE, HSIZE, STEPX, STEPY, SAMPLER };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::Blur:
//
// Constructor.
//
Blur::Blur(QWidget *parent) : ImageFilter(parent)
{
	m_nPasses = 2;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Blur::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Blur");

	// layout for assembling filter widget
	QGridLayout *layout = new QGridLayout;

	// alloc array of labels
	QLabel *label[2];

	// create sliders and spinboxes
	for(int i=0; i<2; i++) {
		// create label[i]
		label[i] = new QLabel(m_ctrlGrp);
		if(!i) label[i]->setText("Width");
		else   label[i]->setText("Height");

		// create slider
		m_slider [i] = new QSlider(Qt::Horizontal, m_ctrlGrp);
		m_slider [i]->setRange(1, 30);
		m_slider [i]->setValue(3);
		m_slider [i]->setSingleStep(2);
		m_slider [i]->setTickPosition(QSlider::TicksBelow);
		m_slider [i]->setTickInterval(5);

		// create spinbox
		m_spinBox[i] = new QSpinBox(m_ctrlGrp);
		m_spinBox[i]->setRange(1, 30);
		m_spinBox[i]->setValue(3);
		m_spinBox[i]->setSingleStep(2);

		// assemble dialog
		layout->addWidget(label    [i], i, 0);
		layout->addWidget(m_slider [i], i, 1);
		layout->addWidget(m_spinBox[i], i, 2);
	}

	// create checkbox
	m_checkBox = new QCheckBox("Lock filter dimensions");
	m_checkBox->setCheckState(Qt::Checked);
	m_checkBox_myBlur = new QCheckBox("My Blur");
	m_checkBox_myBlur->setCheckState(Qt::Unchecked);

	// add checkbox to layout
	layout->addWidget(m_checkBox, 2, 1, Qt::AlignHCenter);
	layout->addWidget(m_checkBox_myBlur, 2, 2);

	// init signal/slot connections
	connect(m_slider [0], SIGNAL(valueChanged(int)), this, SLOT(changeFilterW(int)));
	connect(m_spinBox[0], SIGNAL(valueChanged(int)), this, SLOT(changeFilterW(int)));
	connect(m_slider [1], SIGNAL(valueChanged(int)), this, SLOT(changeFilterH(int)));
	connect(m_spinBox[1], SIGNAL(valueChanged(int)), this, SLOT(changeFilterH(int)));
	connect(m_checkBox  , SIGNAL(stateChanged(int)), this, SLOT(setLock(int)));
	connect(m_checkBox_myBlur, SIGNAL(stateChanged(int)), this, SLOT(setMyBlur(int)));

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Blur::applyFilter(ImagePtr I1, bool gpuFlag, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// collect parameters
	int w = m_slider[0]->value();	// filter width
	int h = m_slider[1]->value();	// filter height
	m_width  = I1->width();
	m_height = I1->height();
	// apply blur filter
	if(!(gpuFlag && m_shaderFlag))
		blur(I1, w, h, I2);	// apply CPU based filter
	else    g_mainWindowP->glw()->applyFilterGPU(m_nPasses);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::blur:
//
// Blur image I1 with a box filter (unweighted averaging).
// The filter has width w and height h.
// Output is in I2.
//
void
Blur::blur(ImagePtr I1, int w, int h, ImagePtr I2)
{
	HW_blur(I1, w, h, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::changeFilterW:
//
// Slot to process change in filter width caused by moving the slider.
//
void
Blur::changeFilterW(int value)
{
	// set value of m_slider[0] and tie it to m_slider[1] if necessary
	for(int i=0; i<2; i++) {
		m_slider [i]->blockSignals(true);
		m_slider [i]->setValue    (value);
		m_slider [i]->blockSignals(false);
		m_spinBox[i]->blockSignals(true);
		m_spinBox[i]->setValue    (value);
		m_spinBox[i]->blockSignals(false);

		// don't tie slider values if lock checkbox is not checked
		if(m_checkBox->checkState() != Qt::Checked) break;
	}

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::changeFilterH:
//
// Slot to process change in filter height caused by moving the slider.
//
void
Blur::changeFilterH(int value)
{
	// set value of m_slider[1] and tie it to m_slider[0] if necessary
	for(int i=1; i>=0; i--) {
		m_slider [i]->blockSignals(true);
		m_slider [i]->setValue    (value);
		m_slider [i]->blockSignals(false);
		m_spinBox[i]->blockSignals(true);
		m_spinBox[i]->setValue    (value);
		m_spinBox[i]->blockSignals(false);

		// don't tie slider values if lock checkbox is not checked
		if(m_checkBox->checkState() != Qt::Checked) break;
	}

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretchDialog::setLock:
//
// Slot to process state change of "Lock" checkbox.
// Set both sliders to same (min) value.
//
void
Blur::setLock(int state)
{
	if(state == Qt::Checked) {
		int val = MIN(m_slider[0]->value(), m_slider[1]->value());
		for(int i=0; i<2; i++) {
			m_slider[i]->blockSignals(true);
			m_slider[i]->setValue(val);
			m_slider[i]->blockSignals(false);
		}
	}

	// apply filter to source image and display result
	g_mainWindowP->preview();
}

void
Blur::setMyBlur(int state)
{
    if(state == Qt::Checked) m_nPasses = 1;
    else m_nPasses = 2;

    // apply filter to source image and display result
    g_mainWindowP->preview();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::reset:
//
// Reset parameters.
//
void
Blur::reset()
{
	m_slider[0]->setValue(3);
	m_slider[1]->setValue(3);
	m_checkBox->setCheckState(Qt::Checked);

	// apply filter to source image and display result
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::initShader:
//
// init shader program and parameters.
//
void
Blur::initShader()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	UniformMap uniforms;

	QString v_name = ":/vshader_passthrough";
	QString f_name = ":/ops_neighborhood/fshader_myblur";

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_Wsize"  ] = WSIZE;
	uniforms["u_Hsize"  ] = HSIZE;
	uniforms["u_StepX"  ] = STEPX;
	uniforms["u_StepY"  ] = STEPY;
	uniforms["u_Sampler"] = SAMPLER;


#ifdef __APPLE__
        v_name += "_Mac";
        f_name += "_Mac";
#endif

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	g_mainWindowP->glw()->initShader(m_program[ONEPASS],
	                                 v_name + ".glsl",
	                                 f_name + ".glsl",
					 											 	 uniforms,
					 											 	 m_uniform[ONEPASS]);
	uniforms.clear();

	v_name = ":/vshader_passthrough";
	f_name = ":/ops_neighborhood/fshader_blur1";

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_Wsize"  ] = WSIZE;
	uniforms["u_Step"   ] = STEPX;
	uniforms["u_Sampler"] = SAMPLER;

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
	    uniforms.clear();

	uniforms["u_Hsize"  ] = HSIZE;
	uniforms["u_Step"   ] = STEPX;
	uniforms["u_Sampler"] = SAMPLER;

  v_name = ":/vshader_passthrough";
	f_name = ":/ops_neighborhood/fshader_blur2";

#ifdef __APPLE__
        v_name += "_Mac";
        f_name += "_Mac";
#endif

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	g_mainWindowP->glw()->initShader(m_program[PASS2],
	                                 v_name + ".glsl",
	                                 f_name + ".glsl",
																	 uniforms,
																	 m_uniform[PASS2]);

	m_shaderFlag = true;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blur::gpuProgram:
//
// Active Blur gpu program
//
void
Blur::gpuProgram(int pass)
{
	int w_size = m_slider[0]->value();
	int h_size = m_slider[1]->value();
	if(w_size % 2 == 0) ++w_size;
	if(h_size % 2 == 0) ++h_size;

	if(m_nPasses == 1) {
		glUseProgram(m_program[ONEPASS].programId());
		glUniform1i (m_uniform[ONEPASS][WSIZE], w_size);
		glUniform1i (m_uniform[ONEPASS][HSIZE], h_size);
		glUniform1f (m_uniform[ONEPASS][STEPX], (GLfloat) 1.0f / m_width);
		glUniform1f (m_uniform[ONEPASS][STEPY], (GLfloat) 1.0f / m_height);
		glUniform1i (m_uniform[ONEPASS][SAMPLER], 0);
	} else {
		switch(pass) {
			case PASS1:
				glUseProgram(m_program[PASS1].programId());
				glUniform1i (m_uniform[PASS1][WSIZE], w_size);
				glUniform1f (m_uniform[PASS1][STEPX],  (GLfloat) 1.0f / m_width);
				glUniform1i (m_uniform[PASS1][SAMPLER], 0);
				break;
			case PASS2:
				glUseProgram(m_program[PASS2].programId());
				glUniform1i (m_uniform[PASS2][HSIZE], h_size);
				glUniform1f (m_uniform[PASS2][STEPX],  (GLfloat) 1.0f / m_height);
				glUniform1i (m_uniform[PASS2][SAMPLER], 0);
				break;
		}
	}
}
