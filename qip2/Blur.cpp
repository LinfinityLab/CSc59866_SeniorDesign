//
//  Blur.cpp
//  qip
//
//  Created by Weifan Lin on 4/4/16.
//
//

#include "Blur.h"
#include "MainWindow.h"

extern MainWindow *g_mainWindowP;


Blur::Blur(QWidget *parent) : ImageFilter(parent)
{}



bool
Blur::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    int xsz = m_sliderX->value();
    int ysz = m_sliderY->value();

    // error checking
    if(xsz < 0 || xsz > MXGRAY || ysz < 0 || ysz > MXGRAY) return 0;
    
    // apply filter
    blur(I1, xsz, ysz, I2);
    
    return 1;
}


QGroupBox*
Blur::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Blur");
    
    // init widgets
    // create label[i]
    QLabel *labelXsz = new QLabel;
    labelXsz->setText(QString("Xsz"));
    QLabel *labelYsz = new QLabel;
    labelYsz->setText(QString("Ysz"));
    QLabel *labelLock = new QLabel;
    labelLock->setText(QString("Synchronize Xsz & Ysz"));
    QLabel *labelEdge = new QLabel;
    labelEdge->setText(QString("Edge Image"));
    QLabel *labelShrp = new QLabel;
    labelShrp->setText(QString("Sharpen"));
    QLabel *labelFctr = new QLabel;
    labelFctr->setText(QString("Fctr"));
    
    
    // create sliders
    m_sliderX = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderX->setTickPosition(QSlider::TicksBelow);
    m_sliderX->setTickInterval(7);
    m_sliderX->setMinimum(1);
    m_sliderX->setMaximum(99);
    m_sliderX->setSingleStep(2);
    m_sliderX->setValue  (1);
    
    m_sliderY = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderY->setTickPosition(QSlider::TicksBelow);
    m_sliderY->setTickInterval(7);
    m_sliderY->setMinimum(1);
    m_sliderY->setMaximum(99);
    m_sliderY->setSingleStep(2);
    m_sliderY->setValue  (1);
    
    m_sliderFctr = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderFctr->setTickPosition(QSlider::TicksBelow);
    m_sliderFctr->setTickInterval(7);
    m_sliderFctr->setMinimum(1);
    m_sliderFctr->setMaximum(99);
    m_sliderFctr->setSingleStep(1);
    m_sliderFctr->setValue  (1);
    
    // create spinboxes
    m_spinBoxX = new QSpinBox(m_ctrlGrp);
    m_spinBoxX->setMinimum(1);
    m_spinBoxX->setMaximum(99);
    m_spinBoxX->setSingleStep(2);
    m_spinBoxX->setValue  (1);
    
    m_spinBoxY = new QSpinBox(m_ctrlGrp);
    m_spinBoxY->setMinimum(1);
    m_spinBoxY->setMaximum(99);
    m_spinBoxY->setSingleStep(2);
    m_spinBoxY->setValue  (1);
    
    m_spinBoxFctr = new QSpinBox(m_ctrlGrp);
    m_spinBoxFctr->setMinimum(1);
    m_spinBoxFctr->setMaximum(99);
    m_spinBoxFctr->setSingleStep(1);
    m_spinBoxFctr->setValue  (1);
    
    // create checkboxes
    m_checkBoxSync = new QCheckBox(m_ctrlGrp);
    m_checkBoxSync->setChecked(true);
    
    m_checkBoxEdge = new QCheckBox(m_ctrlGrp);
    m_checkBoxEdge->setChecked(false);
    
    m_checkBoxShrp = new QCheckBox(m_ctrlGrp);
    m_checkBoxShrp->setChecked(false);
    
    connect(m_sliderX ,     SIGNAL(valueChanged(int)), this, SLOT(changeXsz (int)));
    connect(m_spinBoxX,     SIGNAL(valueChanged(int)), this, SLOT(changeXsz (int)));
    connect(m_sliderY ,     SIGNAL(valueChanged(int)), this, SLOT(changeYsz (int)));
    connect(m_spinBoxY,     SIGNAL(valueChanged(int)), this, SLOT(changeYsz (int)));
    connect(m_checkBoxSync, SIGNAL(stateChanged(int)), this, SLOT(changeSync(int)));
    connect(m_checkBoxEdge, SIGNAL(stateChanged(int)), this, SLOT(changeEdge(int)));
    connect(m_checkBoxShrp, SIGNAL(stateChanged(int)), this, SLOT(changeShrp(int)));
    connect(m_sliderFctr ,  SIGNAL(valueChanged(int)), this, SLOT(changeFctr(int)));
    connect(m_spinBoxFctr,  SIGNAL(valueChanged(int)), this, SLOT(changeFctr(int)));

    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget( labelXsz     , 0, 0);
    layout->addWidget(m_sliderX     , 0, 1);
    layout->addWidget(m_spinBoxX    , 0, 2);
    layout->addWidget( labelYsz     , 1, 0);
    layout->addWidget(m_sliderY     , 1, 1);
    layout->addWidget(m_spinBoxY    , 1, 2);
    layout->addWidget(m_checkBoxSync, 2, 0);
    layout->addWidget(labelLock     , 2, 1);
    layout->addWidget(m_checkBoxEdge, 3, 0);
    layout->addWidget(labelEdge     , 3, 1);
    layout->addWidget(m_checkBoxShrp, 4, 0);
    layout->addWidget(labelShrp     , 4, 1);
    
    layout->addWidget( labelFctr     , 5, 0);
    layout->addWidget(m_sliderFctr    , 5, 1);
    layout->addWidget(m_spinBoxFctr    , 5, 2);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}


void
Blur::changeXsz(int xsz)
{
    if (xsz%2==0) xsz=xsz+1; //need this because slider->getSingleStep(2) has no effect on mouse move
    
    m_sliderX ->blockSignals(true);
    m_sliderX ->setValue    (xsz );
    m_sliderX ->blockSignals(false);
    m_spinBoxX->blockSignals(true);
    m_spinBoxX->setValue    (xsz );
    m_spinBoxX->blockSignals(false);
    
    if (m_checkBoxSync->isChecked()) {
        m_sliderY ->blockSignals(true);
        m_sliderY ->setValue    (xsz );
        m_sliderY ->blockSignals(false);
        m_spinBoxY->blockSignals(true);
        m_spinBoxY->setValue    (xsz );
        m_spinBoxY->blockSignals(false);
    }
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}

void
Blur::changeYsz(int ysz)
{
    if (ysz%2==0) ysz=ysz+1;
    
    m_sliderY ->blockSignals(true);
    m_sliderY ->setValue    (ysz );
    m_sliderY ->blockSignals(false);
    m_spinBoxY->blockSignals(true);
    m_spinBoxY->setValue    (ysz );
    m_spinBoxY->blockSignals(false);
    
    if (m_checkBoxSync->isChecked()) {
        m_sliderX ->blockSignals(true);
        m_sliderX ->setValue    (ysz );
        m_sliderX ->blockSignals(false);
        m_spinBoxX->blockSignals(true);
        m_spinBoxX->setValue    (ysz );
        m_spinBoxX->blockSignals(false);
    }
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}

void
Blur::changeSync(int)
{
    int xsz = m_sliderX->value();
    int ysz = m_sliderY->value();
    
    if (m_checkBoxSync->isChecked()) {
        if (xsz>ysz) changeYsz(xsz);
        else         changeXsz(ysz);
    }
}

void
Blur::changeEdge(int)
{
    // check Edge will uncheck Shrp
    if (m_checkBoxEdge->isChecked() == true) m_checkBoxShrp->setChecked(false);
}

void
Blur::changeShrp(int)
{
    // everytime changeShrp will set Fctr to 1
    m_sliderFctr ->blockSignals(true);
    m_sliderFctr ->setValue    (1);
    m_sliderFctr ->blockSignals(false);
    m_spinBoxFctr->blockSignals(true);
    m_spinBoxFctr->setValue    (1);
    m_spinBoxFctr->blockSignals(false);
    
    // check Shrp will unchekc Edge
    if (m_checkBoxShrp->isChecked() == true) m_checkBoxEdge->setChecked(false);
    
}

void
Blur::changeFctr(int value) {
    
    if (m_checkBoxShrp->isChecked() == false) m_checkBoxShrp->setChecked(true);
    
    m_sliderFctr ->blockSignals(true);
    m_sliderFctr ->setValue    (value);
    m_sliderFctr ->blockSignals(false);
    m_spinBoxFctr->blockSignals(true);
    m_spinBoxFctr->setValue    (value);
    m_spinBoxFctr->blockSignals(false);
}


void
Blur::blur(ImagePtr I1, int xsz, int ysz, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    
    ImagePtr I3; // intermediate buffer
    IP_copyImageHeader(I1, I3);
    
    int w = I1->width();
    int h = I1->height();
    int total = w * h;

    int type;
    ChannelPtr<uchar> p1, p2, p3, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        IP_getChannel(I3, ch, p3, type);
        
        if (xsz == 1) for(endd = p1 + total; p1<endd;) *p3++ = *p1++;
        if (xsz > 1) {
            // blur rows one by one
            for (int y=0; y<h; y++) {
                IP_blur1D(p1, w, xsz, 1, p3);
                p1+=w;
                p3+=w;
            }
        }

        p3=p3-total; // point back to 0
        
        if (ysz == 1) for(endd = p3 + total; p3<endd;) *p2++ = *p3++;
        if (ysz > 1) {
            // blur columns one by one
            for (int x=0; x<w; x++) {
                IP_blur1D(p3, h, ysz, w, p2);
                p3+=1;
                p2+=1;
            }
        }
    }
}

void
Blur::IP_blur1D(ChannelPtr<uchar> &src, int size, int kernel, int stride, ChannelPtr<uchar> &dst) {
    int neighborSz = kernel/2;
    int newSz = size+kernel-1; // this is size for padded buffer
    short* buffer = new short[newSz];
    
    // copy to buffer
    for (int i=0; i<neighborSz; i++) buffer[i] = *src;
    int index = 0;
    for (int i=neighborSz; i < size+neighborSz; i++) {
        buffer[i] = src[index];
        index+=stride;
    }
    for (int i=size+neighborSz; i<newSz; i++) buffer[i] = src[index-stride];
    
    unsigned short sum = 0;
    for(int i=0; i<kernel; i++) sum+=buffer[i];
    
    for (int i=0; i<size; i++) {
        dst[i*stride] = sum/kernel;
        sum+=(buffer[i+kernel] - buffer[i]);
    }
    
    // just for debugging ..
//    for (int i=0; i<size; i++) {
//        p3[i] = buffer[i+neighborSz];
//    }
    
}


void
Blur::reset() {
    changeXsz (1);
    changeYsz (1);
    changeFctr(1);
    m_checkBoxSync->setChecked(true) ;
    m_checkBoxEdge->setChecked(false);
    m_checkBoxShrp->setChecked(false);
}
