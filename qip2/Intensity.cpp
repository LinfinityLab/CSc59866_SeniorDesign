//
//  Intensity.cpp
//  qip
//
//  Created by Weifan Lin on 3/17/16.
//
//

#include "MainWindow.h"
#include "Intensity.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Intensity:Intensity
//
// Constructor.
//
Intensity::Intensity(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Intensity::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    // get intensity value
    int min = m_sliderMin->value();
    int max = m_sliderMax->value();
    
    // error checking
    if(min < 0 || min > MXGRAY || max < 0 || max > MXGRAY) return 0;
    
    // apply filter
    intensity(I1, min, max, I2);
    
    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Intensity::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Intensity::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Intensity Scaling");
    
    // init widgets
    // create label[i]
    QLabel *label_min = new QLabel;
    label_min->setText(QString("min"));
    
    QLabel *label_max = new QLabel;
    label_max->setText(QString("max"));
    
    QLabel *label_auto = new QLabel;
    label_auto->setText(QString("auto"));
    
    // slider for min
    m_sliderMin = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderMin->setTickPosition(QSlider::TicksBelow);
    m_sliderMin->setTickInterval(25);
    m_sliderMin->setMinimum(1);
    m_sliderMin->setMaximum(MXGRAY);
    m_sliderMin->setValue  (DEFAULTMIN);
    
    // slider for max
    m_sliderMax = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderMax->setTickPosition(QSlider::TicksBelow);
    m_sliderMax->setTickInterval(25);
    m_sliderMax->setMinimum(1);
    m_sliderMax->setMaximum(MXGRAY);
    m_sliderMax->setValue  (DEFAULTMAX);
    
    
    // spinbox for min
    m_spinBoxMin = new QSpinBox(m_ctrlGrp);
    m_spinBoxMin->setMinimum(1);
    m_spinBoxMin->setMaximum(MXGRAY);
    m_spinBoxMin->setValue  (DEFAULTMIN);
    
    
    // spinbox for max
    m_spinBoxMax = new QSpinBox(m_ctrlGrp);
    m_spinBoxMax->setMinimum(1);
    m_spinBoxMax->setMaximum(MXGRAY);
    m_spinBoxMax->setValue  (DEFAULTMAX);
    
    // checkbox for auto
    m_checkBox = new QCheckBox(m_ctrlGrp);
    m_checkBox->setChecked(false);
    
    
    // init signal/slot connections for Intensity
    connect(m_sliderMin , SIGNAL(valueChanged(int)), this, SLOT(changeMin  (int)));
    connect(m_spinBoxMin, SIGNAL(valueChanged(int)), this, SLOT(changeMin  (int)));
    connect(m_sliderMax , SIGNAL(valueChanged(int)), this, SLOT(changeMax  (int)));
    connect(m_spinBoxMax, SIGNAL(valueChanged(int)), this, SLOT(changeMax  (int)));
    connect(m_checkBox,   SIGNAL(stateChanged(int)), this, SLOT(changeAuto (int)));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label_min      , 0, 0);
    layout->addWidget(m_sliderMin    , 0, 1);
    layout->addWidget(m_spinBoxMin   , 0, 2);
    layout->addWidget(label_max      , 1, 0);
    layout->addWidget(m_sliderMax    , 1, 1);
    layout->addWidget(m_spinBoxMax   , 1, 2);
    
    layout->addWidget(label_auto, 2, 0);
    layout->addWidget(m_checkBox, 2, 1);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Intensity::changeMin
//
// Slot to process change in min caused by moving the slider.
//
void
Intensity::changeMin(int min)
{
    m_checkBox->blockSignals(true);
    m_checkBox->setCheckState(Qt::Unchecked);
    m_checkBox->blockSignals(false);

    m_sliderMin ->blockSignals(true);
    m_sliderMin ->setValue    (min );
    m_sliderMin ->blockSignals(false);
    m_spinBoxMin->blockSignals(true);
    m_spinBoxMin->setValue    (min );
    m_spinBoxMin->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Intensity::changeMax
//
// Slot to process change in max caused by moving the slider.
//
void
Intensity::changeMax(int max)
{
    m_checkBox->blockSignals(true);
    m_checkBox->setCheckState(Qt::Unchecked);
    m_checkBox->blockSignals(false);
    
    m_sliderMax ->blockSignals(true);
    m_sliderMax ->setValue    (max );
    m_sliderMax ->blockSignals(false);
    m_spinBoxMax->blockSignals(true);
    m_spinBoxMax->setValue    (max );
    m_spinBoxMax->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}

void
Intensity::autoSliderAndSpinBox(QSlider* slider, QSpinBox* spinbox, int value) {
    slider->blockSignals(true );
    slider->setValue    (value);
    slider->blockSignals(false);
    
    spinbox->blockSignals(true );
    spinbox->setValue    (value);
    spinbox->blockSignals(false);
}

void
Intensity::changeAuto(int checked)
{
    ImagePtr I1 = g_mainWindowP->imageSrc();
    ImagePtr I2 = g_mainWindowP->imageDst();
    
    bool isAuto = m_checkBox->isChecked();
    
    int histo[MXGRAY];

    int type;
    ChannelPtr<uchar> p1, p2, endd;
    
    if (isAuto) {
        
        int w = I1->width();
        int h = I1->height();
        int total = w * h;
        
        // initialize histogram array
        for(int i = 0; i < MXGRAY; i++)
            histo[i] = 0;
        
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++)
            for(endd = p1 + total; p1<endd; p1++)
                histo[*p1] ++;
        
        
        // get min
        int min = 0;
        for(int i=0; i<MXGRAY; i++) {
            if (!histo[i]) continue;
            
            min = i;
            qDebug() << min;
            autoSliderAndSpinBox(m_sliderMin, m_spinBoxMin, min);
            break;
        }
        
        // get max
        int max = 255;
        for(int j=MXGRAY-1; j>=0; j--) {
            if (!histo[j]) continue;
            
            max = j;
            qDebug() << max;
            autoSliderAndSpinBox(m_sliderMax, m_spinBoxMax, max);
            break;
        }
        
        for(int i = 0; i < 256; i++)
            qDebug() << i << ": " << histo[i];
        
        
        applyFilter(I1, I2);
        g_mainWindowP->displayOut();
    }
    
}


//
void
Intensity::intensity(ImagePtr I1, int min, int max, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    int diff = 1;
    if (min<max) {
        diff = max-min;
    }
    
    int i, lut[MXGRAY];
    for(i=0; i<MXGRAY; ++i)
        lut[i] = CLIP((int)(255*(i-min))/(diff), 0, 255);
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }
    
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Intensity::reset:
//
// Reset parameters.
//
void
Intensity::reset() {
    changeMin(DEFAULTMIN);
    changeMax(DEFAULTMAX);
}