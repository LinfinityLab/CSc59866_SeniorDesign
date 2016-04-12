//
//  HistogramStretch.cpp
//  qip
//
//  Created by Weifan Lin on 3/17/16.
//
//

#include "MainWindow.h"
#include "HistogramStretch.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistogramStretch:HistogramStretch
//
// Constructor.
//
HistogramStretch::HistogramStretch(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistogramStretch::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
HistogramStretch::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    // get intensity value
    int min = m_sliderMin->value();
    int max = m_sliderMax->value();
    
    // error checking
    if(min < 0 || min > MXGRAY || max < 0 || max > MXGRAY) return 0;
    
    // apply filter
    stretch(I1, min, max, I2);
    
    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistogramStretch::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
HistogramStretch::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Histogram Stretching");
    
    // init widgets
    // create label[i]
    QLabel *label_min = new QLabel;
    label_min->setText(QString("min"));
    
    QLabel *label_max = new QLabel;
    label_max->setText(QString("max"));
    
    QLabel *label_autoMin = new QLabel;
    label_autoMin->setText(QString("auto"));
    QLabel *label_autoMax = new QLabel;
    label_autoMax->setText(QString("auto"));
    
    // slider for min
    m_sliderMin = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderMin->setTickPosition(QSlider::TicksBelow);
    m_sliderMin->setTickInterval(25);
    m_sliderMin->setMinimum(0);
    m_sliderMin->setMaximum(MXGRAY);
    m_sliderMin->setValue  (DEFAULTMIN);
    
    // slider for max
    m_sliderMax = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderMax->setTickPosition(QSlider::TicksBelow);
    m_sliderMax->setTickInterval(25);
    m_sliderMax->setMinimum(0);
    m_sliderMax->setMaximum(MXGRAY);
    m_sliderMax->setValue  (DEFAULTMAX);
    
    
    // spinbox for min
    m_spinBoxMin = new QSpinBox(m_ctrlGrp);
    m_spinBoxMin->setMinimum(0);
    m_spinBoxMin->setMaximum(MXGRAY);
    m_spinBoxMin->setValue  (DEFAULTMIN);
    
    
    // spinbox for max
    m_spinBoxMax = new QSpinBox(m_ctrlGrp);
    m_spinBoxMax->setMinimum(0);
    m_spinBoxMax->setMaximum(MXGRAY);
    m_spinBoxMax->setValue  (DEFAULTMAX);
    
    // checkbox for auto min
    m_checkBoxMin = new QCheckBox(m_ctrlGrp);
    m_checkBoxMin->setChecked(false);
    
    
    // checkbox for auto max
    m_checkBoxMax = new QCheckBox(m_ctrlGrp);
    m_checkBoxMax->setChecked(false);
    
    // init signal/slot connections for Intensity
    connect(m_sliderMin , SIGNAL(valueChanged(int)), this, SLOT(changeMin  (int)));
    connect(m_spinBoxMin, SIGNAL(valueChanged(int)), this, SLOT(changeMin  (int)));
    connect(m_sliderMax , SIGNAL(valueChanged(int)), this, SLOT(changeMax  (int)));
    connect(m_spinBoxMax, SIGNAL(valueChanged(int)), this, SLOT(changeMax  (int)));
    connect(m_checkBoxMin,SIGNAL(stateChanged(int)), this, SLOT(autoMin    (int)));
    connect(m_checkBoxMax,SIGNAL(stateChanged(int)), this, SLOT(autoMax    (int)));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label_min      , 0, 0);
    layout->addWidget(m_sliderMin    , 0, 1);
    layout->addWidget(m_spinBoxMin   , 0, 2);
    layout->addWidget(m_checkBoxMin  , 0, 3);
    layout->addWidget(label_autoMin  , 0, 4);
    layout->addWidget(label_max      , 1, 0);
    layout->addWidget(m_sliderMax    , 1, 1);
    layout->addWidget(m_spinBoxMax   , 1, 2);
    layout->addWidget(m_checkBoxMax  , 1, 3);
    layout->addWidget(label_autoMax  , 1, 4);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistogramStretch::changeMin
//
// Slot to process change in min caused by moving the slider.
//
void
HistogramStretch::changeMin(int min)
{
    m_checkBoxMin->blockSignals(true);
    m_checkBoxMin->setCheckState(Qt::Unchecked);
    m_checkBoxMin->blockSignals(false);

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
// HistogramStretch::changeMax
//
// Slot to process change in max caused by moving the slider.
//
void
HistogramStretch::changeMax(int max)
{
    m_checkBoxMax->blockSignals(true);
    m_checkBoxMax->setCheckState(Qt::Unchecked);
    m_checkBoxMax->blockSignals(false);
    
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
HistogramStretch::autoSliderAndSpinBox(QSlider* slider, QSpinBox* spinbox, int value) {
    slider->blockSignals(true );
    slider->setValue    (value);
    slider->blockSignals(false);
    
    spinbox->blockSignals(true );
    spinbox->setValue    (value);
    spinbox->blockSignals(false);
}

void
HistogramStretch::autoMin(int)
{
    ImagePtr I1 = g_mainWindowP->imageSrc();
    ImagePtr I2 = g_mainWindowP->imageDst();
    
    bool isAutoMin = m_checkBoxMin->isChecked();

    int type;
    ChannelPtr<uchar> p1, p2, endd;
    
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    int histo[MXGRAY];
    // initialize histogram array
    for(int i = 0; i < MXGRAY; i++)
        histo[i] = 0;
    
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++)
        for(endd = p1 + total; p1<endd; p1++)
            histo[*p1] ++;
    
    if (isAutoMin) {
    
        // get min
        int min = 0;
        for(int i=0; i<MXGRAY; i++) {
            if (!histo[i]) continue;
            
            min = i;
            qDebug() << min;
            autoSliderAndSpinBox(m_sliderMin, m_spinBoxMin, min);
            break;
        }
    }
    
    applyFilter(I1, I2);
    g_mainWindowP->displayOut();
    
}


void
HistogramStretch::autoMax(int)
{
    ImagePtr I1 = g_mainWindowP->imageSrc();
    ImagePtr I2 = g_mainWindowP->imageDst();
    
    bool isAutoMax = m_checkBoxMax->isChecked();
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    int histo[MXGRAY];
    // initialize histogram array
    for(int i = 0; i < MXGRAY; i++)
        histo[i] = 0;
    
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++)
        for(endd = p1 + total; p1<endd; p1++)
            histo[*p1] ++;
    
    if (isAutoMax) {
        
        // get max
        int max = 255;
        for(int j=MXGRAY-1; j>=0; j--) {
            if (!histo[j]) continue;
            
            max = j;
            qDebug() << max;
            autoSliderAndSpinBox(m_sliderMax, m_spinBoxMax, max);
            break;
        }
    }
    
    applyFilter(I1, I2);
    g_mainWindowP->displayOut();
    
}


//
void
HistogramStretch::stretch(ImagePtr I1, int min, int max, ImagePtr I2) {
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
// HistogramStretch::reset:
//
// Reset parameters.
//
void
HistogramStretch::reset() {
    changeMin(DEFAULTMIN);
    changeMax(DEFAULTMAX);
}