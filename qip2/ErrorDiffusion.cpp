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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::Threshold:
//
// Constructor.
//
ErrorDiffusion::ErrorDiffusion(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
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



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::controlPanel:
//
// Create group box for control panel.
//
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



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::changeThr:
//
// Slot to process change in thr caused by moving the slider.
//
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



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::threshold:
//
// Threshold I1 using the 2-level mapping shown below.  Output is in I2.
// val<thr: 0;	 val >= thr: MaxGray (255)
//! \brief	Threshold I1 using the 3-level mapping shown below.
//! \details	Output is in I2. val<t1: g1; t1<=val<t2: g2; t2<=val: g3
//! \param[in]	I1  - Input image.
//! \param[in]	thr - Threshold.
//! \param[out]	I2  - Output image.
//
void
ErrorDiffusion::errorDiffusion(ImagePtr I1, int thr, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    
    
    unsigned char *buf;
    buf = (unsigned char *) malloc(2 * (w + 2));
    // |0|.....w.....|0||0|.....w.....|0|
    buf[0] = 0;
    buf[w+1] = 0;
    buf[w+2] = 0;
    buf[2*(w+2)-1] = 0;
    
    unsigned char *arrPointers[2];
    
    for(int i=0; i<2; i++) {
        // store pointers in array.
        arrPointers[i] = &buf[i*(w+2)];
    }
    
    unsigned char *in1, *in2;
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        
        // copyRowToCircBuffer(0);
        for(int i = 0; i < w; i++) {
            buf[i+1] = p1[i];
            
        }
        
//        for(int i = 0; i < (w+2); i++){
//            qDebug() << i <<": " << buf[i] ;
//        }
        
        for(int y=0; y<h; y++) {
            
            //copyRowToCircBuffer(y+1);
            for(int i = 0; i < w; i++) {
                if(y%2==0) {
                    buf[i+1] = p1[(y+1)*w+i];
                } else {
                    buf[(w+2)+i+1] = p1[(y+1)*w+i];
                }
                //                *(buf(y+1)*(w+2)+i) = *(p1(y+1)*(w+2)+i);
                //buf[i+1] = p1[i];
//                *(buf+(y+1)*w+i) = *(p1+(y+1)*w+i);
                //*(row1+i) = *(p1+(y+1)*w+i);
            }
            
            in1 = arrPointers[  y   % 2] + 1;
            in2 = arrPointers[(y+1) % 2] + 1;


            for(int x=0; x<w; x++) {
                *p2 = (*in1 < thr) ? 0 : 255;
                short e = *in1 - *p2;
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


//void copyToBuffer(ImagePtr I1, int row, int bufRowsRequired, unsigned char *buffer) {
//    int height = I1->height();
//    int width = I1->width();
//    unsigned char *in, *ptr;
//    in = I1->image();
//    
//    int type;
//    ChannelPtr<uchar> p1, endd;
//    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
//    IP_getChannel(I1, ch, p1, type);
//    
//    int bufStartIndex = (row % bufRowsRequired) * width;
//    
//    ptr = &buffer[bufStartIndex];
//    for (int x=0; x<width; x++) {
//        ptr[0] = in[row*width+x];
//        ptr++;
//    }
//}


void
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threshold::reset:
//
// Reset parameters.
//

ErrorDiffusion::reset() {
    changeThr(MXGRAY>>1);
}
