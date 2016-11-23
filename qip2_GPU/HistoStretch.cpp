// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// HistoStretch.cpp - Histogram Stretch widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "HistoStretch.h"
#include "hw1/HW_histoStretch.cpp"

extern MainWindow *g_mainWindowP;
enum { THR1, THR2, SAMPLER };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::HistoStretch:
//
// Constructor.
//
HistoStretch::HistoStretch(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
HistoStretch::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("HistoStretch");

	// layout for assembling filter widget
	QGridLayout *layout = new QGridLayout;

	// alloc array of labels
	QLabel *label[2];

	// create sliders and spinboxes
	for(int i=0; i<2; i++) {
		// create label[i]
		label[i] = new QLabel(m_ctrlGrp);
		if(!i) label[i]->setText("Min");
		else   label[i]->setText("Max");

		// create slider
		m_slider [i] = new QSlider(Qt::Horizontal, m_ctrlGrp);
		m_slider [i]->setRange(0, MaxGray);
		m_slider [i]->setValue(i* MaxGray);
		m_slider [i]->setTickPosition(QSlider::TicksBelow);
		m_slider [i]->setTickInterval(25);

		// create spinbox
		m_spinBox[i] = new QSpinBox(m_ctrlGrp);
		m_spinBox[i]->setRange(0, MaxGray);
		m_spinBox[i]->setValue(i* MaxGray);

		// assemble dialog
		layout->addWidget(label    [i], i, 0);
		layout->addWidget(m_slider [i], i, 1);
		layout->addWidget(m_spinBox[i], i, 2);
	}

	// create auto-threshold checkboxes
	m_thr1 = new QCheckBox("Auto min");
	m_thr2 = new QCheckBox("Auto max");
	m_thr1->setCheckState(Qt::Unchecked);
	m_thr2->setCheckState(Qt::Unchecked);

	// init flags
	m_thrFlag1 = m_thr1->isChecked();
	m_thrFlag2 = m_thr2->isChecked();

	// init signal/slot connections for sliders and spinboxes
	connect(m_slider [0], SIGNAL(valueChanged(int)), this, SLOT(changeThr1(int)));
	connect(m_spinBox[0], SIGNAL(valueChanged(int)), this, SLOT(changeThr1(int)));
	connect(m_slider [1], SIGNAL(valueChanged(int)), this, SLOT(changeThr2(int)));
	connect(m_spinBox[1], SIGNAL(valueChanged(int)), this, SLOT(changeThr2(int)));

	// init signal/slot connections for m_auto1 and m_auto2
	connect(m_thr1, SIGNAL(stateChanged(int)), this, SLOT(setThrFlag1(int)));
	connect(m_thr2, SIGNAL(stateChanged(int)), this, SLOT(setThrFlag2(int)));

	// add checkboxes to layout
	layout->addWidget(m_thr1, 2, 1, Qt::AlignHCenter);
	layout->addWidget(m_thr2, 3, 1, Qt::AlignHCenter);

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
HistoStretch::applyFilter(ImagePtr I1, bool gpuFlag, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// collect parameters
	int t1 = m_slider[0]->value();	// min
	int t2 = m_slider[1]->value();	// max
	if(t1<0 || t1>t2 || t2>MaxGray)
		return 0;

	// verify if the threshold values should come from histogram instead
	if(m_thrFlag1 || m_thrFlag2) {
		double hmin, hmax;
		ImagePtr II;

		if(I1->imageType() == RGB_IMAGE) {
			// find min and max of histogram (of grayscale version)
			IP_castImage(I1, BW_IMAGE, II=NEWIMAGE);
			IP_minmaxChannel(II, 0, hmin, hmax); 
		} else	IP_minmaxChannel(I1, 0, hmin, hmax); 

		// reset t1 and slider/spinbox if m_thrFlag1 is set
		if(m_thrFlag1) {
			t1 = hmin;
			m_slider [0]->blockSignals(true);
			m_slider [0]->setValue    (t1);
			m_slider [0]->blockSignals(false);
			m_spinBox[0]->blockSignals(true);
			m_spinBox[0]->setValue    (t1);
			m_spinBox[0]->blockSignals(false);
		}

		// reset t2 and slider/spinbox if m_thrFlag2 is set
		if(m_thrFlag2) {
			t2 = hmax;
			m_slider [1]->blockSignals(true);
			m_slider [1]->setValue    (t2);
			m_slider [1]->blockSignals(false);
			m_spinBox[1]->blockSignals(true);
			m_spinBox[1]->setValue    (t2);
			m_spinBox[1]->blockSignals(false);
		}
	}

	// apply filter
	if(!(gpuFlag && m_shaderFlag))
		// apply CPU based filter
		histoStretch(I1, t1, t2, I2);
	else
		g_mainWindowP->glw()->applyFilterGPU(m_nPasses);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::histoStretch:
//
// Apply histogram stretching to I1. Output is in I2.
// Stretch intensity difference from reference value (128) by multiplying
// difference by "contrast" and adding it back to 128. Shift result by
// adding "brightness" value.
//
void
HistoStretch::histoStretch(ImagePtr I1, int t1, int t2, ImagePtr I2)
{
	HW_histoStretch(I1, t1, t2, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::changeThr1:
//
// Slot to process change in thr1 caused by moving the slider.
//
void
HistoStretch::changeThr1(int thr1)
{
	// get the other threshold value
	int thr2 = m_slider[1]->value();

	if(thr1 > thr2) {
		// prevent the two HistStretchs from crossing over
		if(thr2 > 0)
			thr1 = thr2-1;
		else {
			thr1 = thr2++;

			// update thr2 slider
			m_slider [1]->blockSignals(true);
			m_slider [1]->setValue    (thr1);
			m_slider [1]->blockSignals(false);
			m_spinBox[1]->blockSignals(true);
			m_spinBox[1]->setValue    (thr1);
			m_spinBox[1]->blockSignals(false);
		}
	}

	m_slider [0]->blockSignals(true);
	m_slider [0]->setValue    (thr1);
	m_slider [0]->blockSignals(false);
	m_spinBox[0]->blockSignals(true);
	m_spinBox[0]->setValue    (thr1);
	m_spinBox[0]->blockSignals(false);

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::changeThr2:
//
// Slot to process change in thr2 caused by moving the slider.
//
void
HistoStretch::changeThr2(int thr2)
{
	// get the other threshold value
	int thr1 = m_slider[0]->value();
	
	if(thr2 < thr1) {
		// prevent thr1 and thr2 from crossing over
		if(thr1 < IP::MaxGray)
			thr2 = thr1 + 1;
		else {
			thr2 = thr1--;

			// update thr1 slider
			m_slider [0]->blockSignals(true);
			m_slider [0]->setValue    (thr1);
			m_slider [0]->blockSignals(false);
			m_spinBox[0]->blockSignals(true);
			m_spinBox[0]->setValue    (thr1);
			m_spinBox[0]->blockSignals(false);
		}
	}

	m_slider [1]->blockSignals(true);
	m_slider [1]->setValue    (thr2);
	m_slider [1]->blockSignals(false);
	m_spinBox[1]->blockSignals(true);
	m_spinBox[1]->setValue    (thr2);
	m_spinBox[1]->blockSignals(false);

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretchDialog::setThrFlag1:
//
// Slot to process state change of HistStretch "Auto Min" checkbox.
//
void
HistoStretch::setThrFlag1(int state)
{
	m_thrFlag1 = (state == Qt::Checked) ? 1 : 0;

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretchDialog::setThrFlag2:
//
// Slot to process state change of HistStretch "Auto Max" checkbox.
//
void
HistoStretch::setThrFlag2(int state)
{
	m_thrFlag2 = (state == Qt::Checked) ? 1 : 0;

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::reset:
//
// Reset parameters.
//
void
HistoStretch::reset()
{
	m_slider [0]->setValue(0);
	m_spinBox[0]->setValue(0);
	m_slider [1]->setValue(MaxGray);
	m_spinBox[1]->setValue(MaxGray);
	m_thr1->setCheckState(Qt::Unchecked);
	m_thr2->setCheckState(Qt::Unchecked);
	m_thrFlag1 = m_thr1->isChecked();
	m_thrFlag2 = m_thr2->isChecked();

	// apply filter to source image and display result
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistoStretch::initShader:
//
// init shader program and parameters.
//
void
HistoStretch::initShader() 
{
	m_nPasses = 1;
	// initialize GL function resolution for current context
	initializeGLFunctions();

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_Thr1"] = THR1;
	uniforms["u_Thr2"] = THR2;
	uniforms["u_Sampler"] = SAMPLER;

        QString v_name = ":/vshader_passthrough";
        QString f_name = ":/hw1/fshader_histoStretch";
        
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
// HistoStretch::gpuProgram:
//
// Active gpu program
//
void
HistoStretch::gpuProgram(int pass) 
{
	glUseProgram(m_program[pass].programId());
	glUniform1f (m_uniform[pass][THR1], (GLfloat) m_slider[0]->value()/255.0f);
	glUniform1f (m_uniform[pass][THR2], (GLfloat) m_slider[1]->value()/255.0f);
	glUniform1i (m_uniform[pass][SAMPLER], 0);
}