//
//  ErrorDiffusion.cpp
//  qip
//
//  Created by Weifan Lin on 3/30/16.
//
//

#include "ErrorDiffusion.h"
#include "MainWindow.h"

extern MainWindow *g_mainWindowP;


ErrorDiffusion::ErrorDiffusion(QWidget *parent) : ImageFilter(parent)
{}



bool
ErrorDiffusion::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    // get threshold value
    int thr = m_slider->value();
    
    // error checking
    if(thr < 0 || thr > MXGRAY) return 0;
    
    // apply filter
    errorDiffusion(I1, thr, I2);
    
    return 1;
}


QGroupBox*
ErrorDiffusion::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Threshold");
    
    // init widgets
    // create label[i]
    QLabel *label = new QLabel;
    label->setText(QString("Thr"));
    
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
    
    // init signal/slot connections for Threshold
    connect(m_slider , SIGNAL(valueChanged(int)), this, SLOT(changeThr (int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeThr (int)));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(  label  , 0, 0);
    layout->addWidget(m_slider , 0, 1);
    layout->addWidget(m_spinBox, 0, 2);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}


void
ErrorDiffusion::changeThr(int thr)
{
    m_slider ->blockSignals(true);
    m_slider ->setValue    (thr );
    m_slider ->blockSignals(false);
    m_spinBox->blockSignals(true);
    m_spinBox->setValue    (thr );
    m_spinBox->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}



void
ErrorDiffusion::errorDiffusion(ImagePtr I1, int thr, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    
    short* in1;
    short* in2;
    short e;
    
    int bufSz = w + 2;
    short* buffer0 = new short[bufSz];
    short* buffer1 = new short[bufSz];

    int i, lut[MXGRAY];
    for(i=0; i<thr && i<MXGRAY; ++i) lut[i] = 0;
    for(   ; i <= MaxGray;      ++i) lut[i] = MaxGray;
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        
        // copy first row to buffer0
        copyToBuffer(p1, w, buffer0, bufSz);
        
        for(int y=1; y<h; y++) {
            
            if (y % 2 == 0) {
                copyToBuffer(p1, w, buffer0, bufSz);
                in1 = buffer1+1; // +1 to skip the pad
                in2 = buffer0+1;

            } else {
                copyToBuffer(p1, w, buffer1, bufSz);
                in1 = buffer0+1;
                in2 = buffer1+1;
            }

            for(int x=0; x<w; x++) {
                *p2 = (*in1 < thr) ? 0 : 255; //lut[*in1];  not working
                e = *in1 - *p2;
                in1[1 ] += (e*7/16.0);
                in2[-1] += (e*3/16.0);
                in2[0 ] += (e*5/16.0);
                in2[1 ] += (e*1/16.0);
                
                in1++;
                in2++;
                p2++;
            }
        }
    }
}


void
ErrorDiffusion::copyToBuffer(ChannelPtr<uchar> &p1, int width, short* buffer, int bufSz) {
    buffer[0] = *p1; //0?
    buffer[bufSz-1] = *(p1+width-1); //0?
    for (int i = 1; i < bufSz-1; i++) buffer[i] = *p1++;
}

void
ErrorDiffusion::reset() {
    changeThr(MXGRAY>>1);
}
