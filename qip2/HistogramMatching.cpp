//
//  HistogramMatch.cpp
//  qip
//
//  Created by Weifan Lin on 3/29/16.
//
//

#include "MainWindow.h"
#include "HistogramMatch.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistogramStretch:HistogramStretch
//
// Constructor.
//
HistogramMatch::HistogramMatch(QWidget *parent) : ImageFilter(parent)
{}



bool
HistogramMatch::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    // get expon
    int exp = m_slider->value();
    
    // apply filter
    match(I1, exp, I2);
    
    return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistogramStretch::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
HistogramMatch::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Histogram Matching");
    
    // init widgets
    // create label[i]
    QLabel *label = new QLabel;
    label->setText(QString("expon"));
    
    
    m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_slider->setTickPosition(QSlider::TicksBelow);
    m_slider->setTickInterval(40  );
    m_slider->setMinimum     (-100);
    m_slider->setMaximum     (100 );
    m_slider->setValue       (0   );
    
    m_spinBox = new QSpinBox(m_ctrlGrp);
    m_spinBox->setMinimum   (-100);
    m_spinBox->setMaximum   (100 );
    m_spinBox->setValue     (0   );
    m_spinBox->setSingleStep(1   );
    
    
    // init signal/slot connections for Intensity
    connect(m_slider , SIGNAL(valueChanged(int)), this, SLOT(changeExp(int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeExp(int)));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label       , 0, 0);
    layout->addWidget(m_slider    , 0, 1);
    layout->addWidget(m_spinBox   , 0, 2);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}



void
HistogramMatch::changeExp(int exp)
{
    m_slider ->blockSignals(true);
    m_slider ->setValue    (exp );
    m_slider ->blockSignals(false);
    m_spinBox->blockSignals(true);
    m_spinBox->setValue    (exp );
    m_spinBox->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}


void
HistogramMatch::match(ImagePtr I1, int exp, ImagePtr I2) {
    
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    int left[MXGRAY], right[MXGRAY], histo[MXGRAY], reserved[MXGRAY];
    double Histogram_ref[MXGRAY];
    double Havg = 0, scale;
    int i, p, R = 0, Hsum = 0;
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    
    for(i=0; i<MXGRAY; i++) {
        histo[i] = 0; /* clear histogram */
        reserved[i] = 0;
    }
    
    
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++)
        for(endd = p1 + total; p1<endd; p1++)
            histo[*p1] += 1;
    
    for (i = 0; i < MXGRAY; i++) {
        
        //check slider is positive or negative
        if (exp >= 0) {
            Histogram_ref[i] = pow(i/(double)MaxGray, exp);
        } else {
            Histogram_ref[i] = 1 - pow(i/(double)MaxGray, exp*(-1));
        }
        Havg += Histogram_ref[i];
    }
    
    /* normalize h2 to conform with dimensions of I1 */
    scale = (double) total / Havg;
    if(scale != 1)
        for(i=0; i<MXGRAY; i++)
            Histogram_ref[i] *= scale;
    
    for(i=0; i<MXGRAY; i++) {
        left[i] = R; /* left end of interval */
        Hsum += histo[i];
        
        while(Hsum > Histogram_ref[R] && R < MXGRAY - 1) {
            Hsum -= Histogram_ref[R];
            R++;
        }
        
        if(left[i] != R) {
            reserved[R] = Hsum;
        }
        right[i] = R;
    }
    
    for(i=0; i<MXGRAY; i++) histo[i] = 0;
    
    /* visit all input pixels */
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for(endd = p1 + total; p1<endd; p1++, p2++) {
            p = left[*p1];
            if(left[*p1] != right[*p1]) {
                if(histo[p] < Histogram_ref[p] - reserved[p])
                    *p2 = p;
                else
                    *p2 = p = left[*p1] = MIN(p+1, right[*p1]);
                histo[p]++;
            } else {
                if(histo[p] < Histogram_ref[p])
                    *p2 = p;
                else
                    *p2 = p = left[*p1] = MIN(p+1, right[*p1]);
                histo[p]++;
            }
        }
    }
    
}




void
HistogramMatch::reset() {
    changeExp(0);
}