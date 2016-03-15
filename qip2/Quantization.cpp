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
// Threshold::Threshold:
//
// Constructor.
//
Quantization::Quantization(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::applyFilter:
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
    
    // get threshold value
    int qtz = m_slider->value();
    
    int if_d = m_checkBox->checkState();
    
    // error checking
    if(qtz < 0 || qtz > MXGRAY) return 0;
    
    // apply filter
    quantization(I1, qtz, if_d, I2);
    
    return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::controlPanel:
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
    m_slider->setValue  (MXGRAY>>1);

    
    // create spinbox
    m_spinBox = new QSpinBox(m_ctrlGrp);
    m_spinBox->setMinimum(1);
    m_spinBox->setMaximum(MXGRAY);
    m_spinBox->setValue  (MXGRAY>>1);

    
    // create checkbox
    m_checkBox = new QCheckBox(m_ctrlGrp);
    
    
    // init signal/slot connections for Quantization
    connect(m_slider , SIGNAL(valueChanged(int)), this, SLOT(changeQtz (int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeQtz (int)));
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
Quantization::changeDtr(int if_d)
{
    qDebug() << "dither " << if_d;
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}


//
void
Quantization::quantization(ImagePtr I1, int qtz, int if_d, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    // compute lut[]
    int i, lut[MXGRAY];
    int levels = qtz;
    int scale = MXGRAY / levels;
    double bias = scale/2.0;

    // if diter checkbox is unchecked
    if (if_d == 0) {
        for(i=0; i<MXGRAY; i++) lut[i] = int(scale * (i/scale) + bias);
        
        int type;
        ChannelPtr<uchar> p1, p2, endd;
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
        }
        
    }
    // if diter checkbox is checked
    if (if_d == 2) {
        qDebug() << "dither " << if_d;
    }

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Quantization::reset:
//
// Reset parameters.
//
void
Quantization::reset() {
    changeQtz(128);
    changeDtr(0);
}