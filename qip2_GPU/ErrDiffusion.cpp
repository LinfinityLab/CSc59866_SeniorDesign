// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// ErrDiffusion.cpp - Error diffusion class
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "ErrDiffusion.h"
//#include "hw2/HW_errDiffusion.cpp"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::ErrDiffusion:
//
// Constructor.
//
ErrDiffusion::ErrDiffusion(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
ErrDiffusion::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Error Diffusion");

	// alloc array of labels
	QLabel *label[2];
	label[0] = new QLabel("Method");
	label[1] = new QLabel("Gamma");

	// create combobox
	QStringList comboItems;
	comboItems	<< "Floyd-Steinberg"
			<< "Jarvis-Judice-Ninke";
	m_comboBox = new QComboBox(m_ctrlGrp);
	m_comboBox->addItems(comboItems);

	// create slider
	m_slider  = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_slider->setRange(1, 100);
	m_slider->setValue(5);
	m_slider->setTickPosition(QSlider::TicksBelow);
	m_slider->setTickInterval(25);

	// create spinbox
	m_spinBox = new QDoubleSpinBox(m_ctrlGrp);
	m_spinBox->setRange(0.1, 10.);
	m_spinBox->setValue	(0.5);
	m_spinBox->setSingleStep(0.1);
	m_spinBox->setDecimals	(2);

	// create checkbox
	m_checkBox = new QCheckBox("Serpentine");
	m_checkBox-> setCheckState( Qt::Unchecked );

	// init signal/slot connections
	connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMethod(int)));
	connect(m_slider  , SIGNAL(valueChanged( int  )), this, SLOT(changeGammaI( int   )));
	connect(m_spinBox , SIGNAL(valueChanged(double)), this, SLOT(changeGammaD(double )));
	connect(m_checkBox, SIGNAL(stateChanged( int  )), this, SLOT(changeSerpentine(int)));

	// assemble dialog
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0]  , 0, 0);
	layout->addWidget(m_comboBox, 0, 1, 1, 2);
	layout->addWidget(label[1]  , 1, 0);
	layout->addWidget(m_slider  , 1, 1);
	layout->addWidget(m_spinBox , 1, 2);
	layout->addWidget(m_checkBox, 2, 0, 1, 3, Qt::AlignLeft);

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
ErrDiffusion::applyFilter(ImagePtr I1, bool /*gpuFlag*/, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// get error diffusion method selection
	int mtd;
	QString method = m_comboBox->currentText();
	     if(method == "Floyd-Steinberg"    ) mtd = 0;
	else if(method == "Jarvis-Judice-Ninke") mtd = 1;
	else return 0;

	// get gamma value
	double gamma = m_slider->value() * 0.1;

	// error checking
	if(gamma < 0.1 || gamma > 10.0) return 0;

	// get serpentine flag
	bool serpentine = 0;
	if(m_checkBox->checkState() == Qt::Checked)
		serpentine = 1;

	// apply filter
	errDiffusion(I1, mtd, serpentine, gamma, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::errDiffusion:
//
// Dither I1 using error diffusion algorithm specified by method.
// Apply serpentine scan if serpentine flag is set.
// Apply gamma correction to I1 prior to the error diffusion algorithm.
// Output is in I2.
//
void
ErrDiffusion::errDiffusion(ImagePtr I1, int method, bool serpentine, double gamma, ImagePtr I2)
{
//	HW_errDiffusion(I1, method, serpentine, gamma, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::changeMethod:
//
// Slot to process change caused by selecting an error diffusion method.
//
void
ErrDiffusion::changeMethod(int value)
{
	m_comboBox->setCurrentIndex(value);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::changeSerpentine:
//
// Slot to process the checkbox that enables/disables the use of serpentine scanning.
//
void
ErrDiffusion::changeSerpentine(int flag)
{
	int checkState = m_checkBox->checkState();

	if(flag && checkState != Qt::Checked)
		m_checkBox->setCheckState (Qt::Checked);
	else if(!flag && checkState == Qt::Checked)
		m_checkBox->setCheckState (Qt::Unchecked);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::changeGammaI:
//
// Slot to process change in gamma value caused by moving the slider (int)
//
void
ErrDiffusion::changeGammaI(int gamma)
{
	// set spinbox
	m_spinBox->setValue(gamma*0.1);

	// apply filter and display output
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::changeGammaD:
//
// Slot to process change in gamma value caused by moving the spinbox (double)
//
void
ErrDiffusion::changeGammaD(double gamma)
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
// ErrDiffusion::reset:
//
// Reset parameters.
//
void
ErrDiffusion::reset()
{
	m_comboBox->setCurrentIndex(0);
	m_slider ->blockSignals(true);
	m_slider ->setValue    (5);
	m_slider ->blockSignals(false);
	m_spinBox->blockSignals(true);
	m_spinBox->setValue    (.5);
	m_spinBox->blockSignals(false);
	m_checkBox->setCheckState(Qt::Unchecked);

	// apply filter and display output
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::initShader:
//
// init shader program and parameters.
//
void
ErrDiffusion::initShader() {

	m_shaderFlag = false;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ErrDiffusion::gpuProgram:
//
// Active gpu program
//
void
ErrDiffusion::gpuProgram(int ) {

}