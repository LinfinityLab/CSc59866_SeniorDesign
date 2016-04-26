//
//  MedianFilter.cpp
//  qip
//
//  Created by Weifan Lin on 4/11/16.
//
//

#include "MedianFilter.h"
#include "MainWindow.h"
#include <vector>
#include <algorithm>

extern MainWindow *g_mainWindowP;


MedianFilter::MedianFilter(QWidget *parent) : ImageFilter(parent)
{}



bool
MedianFilter::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // error checking
    if(I1.isNull()) return 0;
    
    int nbrSz = m_sliderN->value();
    int k     = m_sliderK->value();
    int reApply = m_sliderR->value();
    
    // error checking
    if(k > nbrSz*nbrSz/2) return 0;
    
    // apply filter
    medianFilter(I1, nbrSz, k, I2);
    if (reApply > 0) {
        for (int i=1; i<=reApply; i++) {
            ImagePtr temp = I2;
            medianFilter(temp, nbrSz, k, I2);
        }
    }
    
    return 1;
}


QGroupBox*
MedianFilter::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Median Filter");
    
    // init widgets
    // create label[i]
    QLabel *labelK = new QLabel;
    labelK->setText(QString("Avg k"));
    
    QLabel *labelNbrSz = new QLabel;
    labelNbrSz->setText(QString("Nbr Sz"));
    
    QLabel *labelTimes = new QLabel;
    labelTimes->setText(QString("Re-apply"));
    
    // create slider for nbr
    m_sliderN = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderN->setTickPosition(QSlider::TicksBelow);
    m_sliderN->setTickInterval(2);
    m_sliderN->setMinimum(1);
    m_sliderN->setMaximum(15);
    m_sliderN->setSingleStep(2);
    m_sliderN->setValue  (1);
    
    // create spinbox for nbr
    m_spinBoxN = new QSpinBox(m_ctrlGrp);
    m_spinBoxN->setMinimum(1);
    m_spinBoxN->setMaximum(15);
    m_spinBoxN->setSingleStep(2);
    m_spinBoxN->setValue  (1);
    
    
    // create slider for k
    m_sliderK = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderK->setTickPosition(QSlider::TicksBelow);
    m_sliderK->setTickInterval(14);
    m_sliderK->setMinimum(0);
    m_sliderK->setMaximum(112);
    m_sliderK->setSingleStep(1);
    m_sliderK->setValue  (0);
    
    // create spinbox for k
    m_spinBoxK = new QSpinBox(m_ctrlGrp);
    m_spinBoxK->setMinimum(0);
    m_spinBoxK->setMaximum(99);
    m_spinBoxK->setSingleStep(1);
    m_spinBoxK->setValue  (0);
    
    // create checkbox for re-apple
    // create slider for k
    m_sliderR = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderR->setTickPosition(QSlider::TicksBelow);
    m_sliderR->setTickInterval(1);
    m_sliderR->setMinimum(0);
    m_sliderR->setMaximum(10);
    m_sliderR->setSingleStep(1);
    m_sliderR->setValue  (0);
    
    // create spinbox for k
    m_spinBoxR = new QSpinBox(m_ctrlGrp);
    m_spinBoxR->setMinimum(0);
    m_spinBoxR->setMaximum(10);
    m_spinBoxR->setSingleStep(1);
    m_spinBoxR->setValue  (0);
    
    // init signal/slot connections for Threshold
    connect(m_sliderN , SIGNAL(valueChanged(int)), this, SLOT(changeNbr (int)));
    connect(m_spinBoxN, SIGNAL(valueChanged(int)), this, SLOT(changeNbr (int)));
    connect(m_sliderK , SIGNAL(valueChanged(int)), this, SLOT(changeK   (int)));
    connect(m_spinBoxK, SIGNAL(valueChanged(int)), this, SLOT(changeK   (int)));
    connect(m_sliderR , SIGNAL(valueChanged(int)), this, SLOT(changeRe  (int)));
    connect(m_spinBoxR, SIGNAL(valueChanged(int)), this, SLOT(changeRe  (int)));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(labelNbrSz, 0, 0);
    layout->addWidget(m_sliderN , 0, 1);
    layout->addWidget(m_spinBoxN, 0, 2);
    
    layout->addWidget(labelK    , 1, 0);
    layout->addWidget(m_sliderK , 1, 1);
    layout->addWidget(m_spinBoxK, 1, 2);
    
    layout->addWidget(labelTimes, 2, 0);
    layout->addWidget(m_sliderR , 2, 1);
    layout->addWidget(m_spinBoxR, 2, 2);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    return(m_ctrlGrp);
}


void
MedianFilter::changeNbr(int nbr)
{
    if (nbr%2==0) nbr=nbr+1;
        
    m_sliderN ->blockSignals(true);
    m_sliderN ->setValue    (nbr );
    m_sliderN ->blockSignals(false);
    m_spinBoxN->blockSignals(true);
    m_spinBoxN->setValue    (nbr );
    m_spinBoxN->blockSignals(false);
    
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    g_mainWindowP->displayOut();
}

void
MedianFilter::changeK(int k)
{
    m_sliderK ->blockSignals(true);
    m_sliderK ->setValue    (k   );
    m_sliderK ->blockSignals(false);
    m_spinBoxK->blockSignals(true);
    m_spinBoxK->setValue    (k   );
    m_spinBoxK->blockSignals(false);
    
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    g_mainWindowP->displayOut();
}

void
MedianFilter::changeRe(int re)
{
    m_sliderR ->blockSignals(true);
    m_sliderR ->setValue    (re  );
    m_sliderR ->blockSignals(false);
    m_spinBoxR->blockSignals(true);
    m_spinBoxR->setValue    (re  );
    m_spinBoxR->blockSignals(false);
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    g_mainWindowP->displayOut();
}


void
MedianFilter::medianFilter(ImagePtr I1, int nbr, int k, ImagePtr I2) {
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w*h;
    
    if (nbr == 1) {
        int type;
        ChannelPtr<uchar> p1, p2, endd;
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            for(endd = p1 + total; p1<endd;) *p2++ = *p1++;
        }
    } else if (nbr > 1) {
        int bufSz = nbr+w-1; // size of buffer for each padded row
        short* buffers[nbr]; // array of nbr pointers, nbr refers to # of pointers
        for (int i=0; i<nbr; i++) { buffers[i] = new short[bufSz]; }
        
        std::vector<int> v(0); // vector for storing neighbors
        
        int type;
        ChannelPtr<uchar> p1, p2, endd;
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            endd = p1+total;

            // copy first row to first nbr/2 buffers, where are top padded rows
            for (int i=0; i<nbr/2; i++) {
                copyOneRowToBuffer(p1, buffers[i], w, nbr);
                p1=p1-w; // we dont want to go to the next row, continue copying the first row
            }
            
            // continue to rest of buffers, note here we still begin copying from frist row of I1
            for (int i=nbr/2; i<nbr; i++) {
                copyOneRowToBuffer(p1, buffers[i], w, nbr);
            }

            for (int y=0; y<h; y++) {
                for (int x=0; x<w; x++) {
                    for (int i=0; i<nbr; i++) {
                        for (int j=0; j<nbr; j++) {
                            v.push_back(buffers[i][j+x]);
                        }
                    }
                    int median = getMedianWithK(v, k);
                    *p2++ = median;
                    v.clear();
                }
                int nextRowIndex = y+nbr-1;
                int nextBufferIndex = nextRowIndex%nbr;
                copyOneRowToBuffer(p1, buffers[nextBufferIndex], w, nbr);
                if (p1>endd) p1-=w; // if have passed last pix, go back to the first pix of last row
            }
        }
    }
}

void
MedianFilter::IP_padImage(ImagePtr src, int padSz, ImagePtr padded) {  // padSz = nbr/2
    int srcW = src->width();
    int paddedW = padded->width();
    int paddedH = padded->height();

    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for(int ch = 0; IP_getChannel(src, ch, p1, type); ch++) {
        IP_getChannel(padded, ch, p2, type);
        for (int i=0; i<padSz; i++) {
            for (int x=0; x<srcW; x++) {
                p2[i*paddedW+x+padSz] = p1[x];
                p2[(i+paddedH-padSz)*paddedW+x+padSz] = p1[x];
            }
            for (int y=0; y<paddedH; y++) {
                p2[y*paddedW+i] = p1[y*srcW];
                p2[y*paddedW+srcW+padSz+i-1] = p1[(y+1)*srcW-1];
            }
        }
        for (int y=padSz; y<paddedH-padSz; y++) {
            for (int x=padSz; x<paddedW-padSz; x++) {
                p2[y*paddedW+x] = p1[(y-padSz)*srcW+x-padSz];
            }
        }
    }
}

void
MedianFilter::copyOneRowToBuffer(ChannelPtr<uchar> &p1, short* bufFor1Row, int w, int nbr) {
    /* |..nbr/2..|..w..|..nbr/2..|  => bufSz=nbr+w-1 */
    
    int bufSz = nbr+w-1;
    for (int i=0; i<nbr/2; i++) { bufFor1Row[i]=*p1; }
    for (int i=nbr/2; i<nbr/2+w; i++) {
        bufFor1Row[i] = *p1;
        p1++;
    }
    p1-=1;
    for (int i=nbr/2+w; i<bufSz; i++) { bufFor1Row[i]=*p1; }
    p1+=1;
}

int
MedianFilter::getMedianWithK(std::vector<int> v, int k) {
    int vSz = v.size(); // vector size, also equal to kernel size, always odd
    int median;
    unsigned short sum = 0;
    
    if (vSz==1) { median = v[0]; }
    else {
        std::sort(v.begin(), v.end()); // sort the vector
        
        int middleIndex = vSz/2;
        if (k==0) { median = v[middleIndex]; }
        else {
            sum+=v[middleIndex];
            for (int i=1; i<=k; i++) { sum+= (v[middleIndex-i]+v[middleIndex+i]); }
            median = sum/(k*2+1);
        }
    }
    return median;
}

void
MedianFilter::reset() {
    changeNbr(1);
    changeK(0);
    changeRe(0);
}
