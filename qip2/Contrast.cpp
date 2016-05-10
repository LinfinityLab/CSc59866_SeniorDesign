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

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
Contrast::Contrast(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//

bool
Contrast::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // INSERT YOUR CODE HERE
    
    if(I1.isNull()) return 0;
    
    // get value
    double brigt, contr, refce;	// brightness, contrast parameters
    brigt = (double) m_sliderB->value();
    contr = (double) m_sliderC->value();
    refce = (double) m_sliderR->value();
    
    // error checking
    if(brigt < -256 || brigt > 256 || contr < -100 || contr >100) return 0;
    
    // apply filter
    contrast(I1, brigt, contr, refce, I2);
    
    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Contrast::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Contrast");
    
    // label for brightness
    QLabel *label_brightness = new QLabel;
    label_brightness->setText(QString("Brightness"));
    
    // slider for brightness
    m_sliderB = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderB->setTickPosition(QSlider::TicksBelow);
    m_sliderB->setTickInterval(25);
    m_sliderB->setMinimum(-256);
    m_sliderB->setMaximum(256);
    m_sliderB->setValue  (0);
    
    // spinbox for brightness
    m_spinBoxB = new QSpinBox(m_ctrlGrp);
    m_spinBoxB->setMinimum(-256);
    m_spinBoxB->setMaximum(256);
    m_spinBoxB->setValue  (0);
    
    // label for contrast
    QLabel *label_contrast = new QLabel;
    label_contrast->setText(QString("Contrast"));
    
    // slider for contrast
    m_sliderC = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderC->setTickPosition(QSlider::TicksBelow);
    m_sliderC->setTickInterval(25);
    m_sliderC->setMinimum(-100);
    m_sliderC->setMaximum(100);
    m_sliderC->setValue  (0);
    
    // doublespinbox for contrast
    m_spinBoxC = new QDoubleSpinBox(m_ctrlGrp);
    m_spinBoxC->setMinimum(-100);
    m_spinBoxC->setMaximum(100);
    m_spinBoxC->setValue  (0);
    m_spinBoxC->setSingleStep(0.5);
    
    // label for reference
    QLabel *label_reference = new QLabel;
    label_reference->setText(QString("Reference"));
    
    // slider for reference
    m_sliderR = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderR->setTickPosition(QSlider::TicksBelow);
    m_sliderR->setTickInterval(25);
    m_sliderR->setMinimum(0);
    m_sliderR->setMaximum(256);
    m_sliderR->setValue  (128);
    
    // spinbox for reference
    m_spinBoxR = new QSpinBox(m_ctrlGrp);
    m_spinBoxR->setMinimum(0);
    m_spinBoxR->setMaximum(256);
    m_spinBoxR->setValue  (128);
    
    // init signal/slot connections for brightness, contrast and reference
    connect(m_sliderB , SIGNAL(valueChanged(int   )), this, SLOT(changeBrightness (int   )));
    connect(m_spinBoxB, SIGNAL(valueChanged(int   )), this, SLOT(changeBrightness (int   )));
    connect(m_sliderC,  SIGNAL(valueChanged(int   )), this, SLOT(changeContrast2  (int   )));
    connect(m_spinBoxC, SIGNAL(valueChanged(double)), this, SLOT(changeContrast   (double)));
    connect(m_sliderR,  SIGNAL(valueChanged(int   )), this, SLOT(changeReference  (int   )));
    connect(m_spinBoxR, SIGNAL(valueChanged(int   )), this, SLOT(changeReference  (int   )));
    
    // INSERT YOUR CODE HERE
    
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label_brightness, 0, 0);
    layout->addWidget(m_sliderB,        0, 1);
    layout->addWidget(m_spinBoxB,       0, 2);
    
    layout->addWidget(label_contrast,   1, 0);
    layout->addWidget(m_sliderC,        1, 1);
    layout->addWidget(m_spinBoxC,       1, 2);
    
    layout->addWidget(label_reference,  2, 0);
    layout->addWidget(m_sliderR,        2, 1);
    layout->addWidget(m_spinBoxR,       2, 2);
    
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}


void Contrast::changeContrast(double contrast)
{
    m_sliderC ->blockSignals(true );
    m_sliderC ->setValue    (int(contrast));
    m_sliderC ->blockSignals(false);
    m_spinBoxC->blockSignals(true );
    m_spinBoxC->setValue    (contrast);
    m_spinBoxC->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}

void Contrast::changeContrast2(int contrast)
{
    m_sliderC ->blockSignals(true );
    m_sliderC ->setValue    (contrast);
    m_sliderC ->blockSignals(false);
    m_spinBoxC->blockSignals(true );
    m_spinBoxC->setValue    (double(contrast));
    m_spinBoxC->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}


void Contrast::changeBrightness(int brightness)
{
    m_sliderB ->blockSignals(true );
    m_sliderB ->setValue    (brightness);
    m_sliderB ->blockSignals(false);
    m_spinBoxB->blockSignals(true );
    m_spinBoxB->setValue    (brightness);
    m_spinBoxB->blockSignals(false);
    
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
    
    
}

void Contrast::changeReference(int reference)
{
    m_sliderR ->blockSignals(true );
    m_sliderR ->setValue    (reference);
    m_sliderR ->blockSignals(false);
    m_spinBoxR->blockSignals(true );
    m_spinBoxR->setValue    (reference);
    m_spinBoxR->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}


void
Contrast::contrast(ImagePtr I1, double brightness, double contrast, double reference, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    double contr;
    
    //set contrast range to 0.25 to 5
    if (contrast >= 0) { contr = contrast / 25.0 + 1.0; }
    else               { contr = contrast / 133.0 + 1.0;}
    
    int i, lut[MXGRAY];
    for(i=0; i<MXGRAY; ++i)
        lut[i] = (int)CLIP((i - reference)*contr + reference + brightness, 0, 255);
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Contrast::reset() {
    
    changeBrightness (0);
    changeContrast   (0);
    changeReference  (128);
}
