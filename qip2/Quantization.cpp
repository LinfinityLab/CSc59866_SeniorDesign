//
//  quantization.cpp
//  qip
//
//  Created by Weifan Lin on 3/14/16.
//
//

#include "MainWindow.h"
#include "Quantization.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantization::Quantization:
//
// Constructor.
//
Quantization::Quantization(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantization::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Quantization::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    int qtz = m_slider->value();
    bool isDither = m_checkBox->isChecked();
    
    // error checking
    if(qtz < 0 || qtz > MXGRAY) return 0;
    
    // apply filter
    quantization(I1, qtz, isDither, I2);
    
    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantization::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
Quantization::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Quantization");
    
    // init widgets
    // create label[i]
    QLabel *label_levels = new QLabel;
    label_levels->setText(QString("levels"));
    
    QLabel *label_dither = new QLabel;
    label_dither->setText(QString("dither"));
    
    // create slider
    m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider->setTickPosition(QSlider::TicksBelow);
    m_slider->setTickInterval(25);
    m_slider->setMinimum(1);
    m_slider->setMaximum(MXGRAY);
    m_slider->setValue  (4);

    
    // create spinbox
    m_spinBox = new QSpinBox(m_ctrlGrp);
    m_spinBox->setMinimum(1);
    m_spinBox->setMaximum(MXGRAY);
    m_spinBox->setValue  (4);

    
    // create checkbox
    m_checkBox = new QCheckBox(m_ctrlGrp);
    
    
    // init signal/slot connections for Quantization
    connect(m_slider  , SIGNAL(valueChanged(int)), this, SLOT(changeQtz (int)));
    connect(m_spinBox , SIGNAL(valueChanged(int)), this, SLOT(changeQtz (int)));
    connect(m_checkBox, SIGNAL(stateChanged(int)), this, SLOT(changeDtr (int)));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label_levels, 0, 0);
    layout->addWidget(m_slider , 0, 1);
    layout->addWidget(m_spinBox, 0, 2);
    
    layout->addWidget(label_dither, 1, 0);
    layout->addWidget(m_checkBox, 1, 1);
    
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantization::changeQtz
//
// Slot to process change in qtz caused by moving the slider.
//
void
Quantization::changeQtz(int qtz)
{
    m_slider ->blockSignals(true);
    m_slider ->setValue    (qtz );
    m_slider ->blockSignals(false);
    m_spinBox->blockSignals(true);
    m_spinBox->setValue    (qtz );
    m_spinBox->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}

void
Quantization::changeDtr(int)
{
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}


//
void
Quantization::quantization(ImagePtr I1, int qtz, bool isDither, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    // compute lut[]
    int i, lut[MXGRAY];
    int levels = qtz;
    int scale = MXGRAY / levels;
    double bias = scale/2.0;

    for(i=0; i<MXGRAY; i++) lut[i] = CLIP(int(scale * (i/scale) + bias), 0, 255);
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    
    if (!isDither){ // if dither checkbox is unchecked
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
        }
    } else {
        int pixelPoint = 0;
        int k;
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            for(endd = p1 + total; p1<endd; p1++, pixelPoint++) {
                if (pixelPoint % 2 == 0)
                    k = CLIP(*p1 + (rand() % (int) bias), 0, 255);
                else
                    k = CLIP(*p1 - (rand() % (int) bias), 0, 255);
                *p2++ = lut[k];
            }
        }
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantization::reset
//
// Reset parameters.
//
void
Quantization::reset() {
    changeQtz(4);
    changeDtr(0);
}