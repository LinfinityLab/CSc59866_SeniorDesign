//
//  Sharpen.cpp
//  qip
//
//  Created by Weifan Lin on 4/12/16.
//
//

#include "Sharpen.h"
#include "MainWindow.h"

extern MainWindow *g_mainWindowP;


Sharpen::Sharpen(QWidget *parent) : Blur(parent)
{}



bool
Sharpen::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    int xsz  = m_sliderX   ->value();
    int ysz  = m_sliderY   ->value();
    int fctr = m_sliderFctr->value();
    
    // error checking
    if(xsz < 0 || xsz > MXGRAY || ysz < 0 || ysz > MXGRAY) return 0;
    
    // apply filter
    blur(I1, xsz, ysz, I2);  // blur function from Blur.cpp
    
    if (m_radioButtonEdge->isChecked()) edge(I1, fctr, I2);
    
    if (m_radioButtonShrp->isChecked()) shrp(I1, fctr, I2);
    
    return 1;
}


QGroupBox*
Sharpen::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Sharpen");
    
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
    
    m_radioButtonEdge = new QRadioButton(m_ctrlGrp);
    m_radioButtonShrp = new QRadioButton(m_ctrlGrp);
    m_radioButtonShrp->setChecked(true);
    
    connect(m_sliderX ,        SIGNAL(valueChanged(int)), this, SLOT(changeXsz (int)));
    connect(m_spinBoxX,        SIGNAL(valueChanged(int)), this, SLOT(changeXsz (int)));
    connect(m_sliderY ,        SIGNAL(valueChanged(int)), this, SLOT(changeYsz (int)));
    connect(m_spinBoxY,        SIGNAL(valueChanged(int)), this, SLOT(changeYsz (int)));
    connect(m_checkBoxSync,    SIGNAL(stateChanged(int)), this, SLOT(changeSync(int)));
    connect(m_radioButtonEdge, SIGNAL(clicked())        , this, SLOT(changeEdge()))   ;
    connect(m_radioButtonShrp, SIGNAL(clicked())        , this, SLOT(changeShrp()))   ;
    connect(m_sliderFctr ,     SIGNAL(valueChanged(int)), this, SLOT(changeFctr(int)));
    connect(m_spinBoxFctr,     SIGNAL(valueChanged(int)), this, SLOT(changeFctr(int)));
    
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(m_checkBoxSync   , 0, 0);
    layout->addWidget(labelLock        , 0, 1);
    layout->addWidget( labelXsz        , 1, 0);
    layout->addWidget(m_sliderX        , 1, 1);
    layout->addWidget(m_spinBoxX       , 1, 2);
    layout->addWidget( labelYsz        , 2, 0);
    layout->addWidget(m_sliderY        , 2, 1);
    layout->addWidget(m_spinBoxY       , 2, 2);
    layout->addWidget(m_radioButtonEdge, 3, 0);
    layout->addWidget(labelEdge        , 3, 1);
    layout->addWidget(m_radioButtonShrp, 4, 0);
    layout->addWidget(labelShrp        , 4, 1);
    layout->addWidget( labelFctr       , 5, 0);
    layout->addWidget(m_sliderFctr     , 5, 1);
    layout->addWidget(m_spinBoxFctr    , 5, 2);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}


void
Sharpen::changeEdge()
{
    if (m_radioButtonEdge->isChecked()) {
        applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
        g_mainWindowP->displayOut();
    }
    
}

void
Sharpen::changeShrp()
{
    if (m_radioButtonShrp->isChecked()) {
        applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
        g_mainWindowP->displayOut();
    }
}

void
Sharpen::changeFctr(int value) {
    
    m_sliderFctr ->blockSignals(true );
    m_sliderFctr ->setValue    (value);
    m_sliderFctr ->blockSignals(false);
    m_spinBoxFctr->blockSignals(true );
    m_spinBoxFctr->setValue    (value);
    m_spinBoxFctr->blockSignals(false);

    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    g_mainWindowP->displayOut();
    
}

void
Sharpen::edge(ImagePtr I1, int fctr, ImagePtr I2) { // edge: I2 = I1-I2
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd;) {
            *p2 = CLIP((*p1 - *p2) * fctr, 0, 255);
            *p1++;
            *p2++;
        }
    }
}

void
Sharpen::shrp(ImagePtr I1, int fctr, ImagePtr I2) { // shrp: I2 = I1+ (I1-I2)*fctr
    
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd;) {
            *p2 = CLIP(*p1 + (*p1 - *p2) * fctr, 0, 255);
            *p1++;
            *p2++;
        }
    }
}


void
Sharpen::reset() {
    changeXsz (1);
    changeYsz (1);
    changeFctr(1);
    m_checkBoxSync->setChecked(true) ;
    m_radioButtonShrp->setChecked(true);
}