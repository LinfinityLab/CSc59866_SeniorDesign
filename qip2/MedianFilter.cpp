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
#include <time.h>

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
    
    QLabel *labelHistoBased = new QLabel;
    labelHistoBased->setText(QString("Histogram Based Filtering"));
    
    QLabel *labelSortMethod = new QLabel;
    labelSortMethod->setText(QString("Sorting Method Filtering"));
    
    
    // create slider for nbr
    m_sliderN = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderN->setTickPosition(QSlider::TicksBelow);
    m_sliderN->setTickInterval(2);
    m_sliderN->setMinimum(1);
    m_sliderN->setMaximum(100);
    m_sliderN->setSingleStep(2);
    m_sliderN->setValue  (1);
    
    // create spinbox for nbr
    m_spinBoxN = new QSpinBox(m_ctrlGrp);
    m_spinBoxN->setMinimum(1);
    m_spinBoxN->setMaximum(100);
    m_spinBoxN->setSingleStep(2);
    m_spinBoxN->setValue  (1);
    
    
    // create slider for k
    m_sliderK = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderK->setTickPosition(QSlider::TicksBelow);
    m_sliderK->setTickInterval(14);
    m_sliderK->setMinimum(0);
    m_sliderK->setMaximum(200);
    m_sliderK->setSingleStep(1);
    m_sliderK->setValue  (0);
    
    // create spinbox for k
    m_spinBoxK = new QSpinBox(m_ctrlGrp);
    m_spinBoxK->setMinimum(0);
    m_spinBoxK->setMaximum(200);
    m_spinBoxK->setSingleStep(1);
    m_spinBoxK->setValue  (0);
    
    // create checkbox for re-apple
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
    
    // create radio buttons
    m_radioButtonSort = new QRadioButton(m_ctrlGrp);
    m_radioButtonHist = new QRadioButton(m_ctrlGrp);
    m_radioButtonSort->setChecked(true);
    
    
    // init signal/slot connections for Threshold
    connect(m_sliderN , SIGNAL(valueChanged(int)), this, SLOT(changeNbr (int)));
    connect(m_spinBoxN, SIGNAL(valueChanged(int)), this, SLOT(changeNbr (int)));
    connect(m_sliderK , SIGNAL(valueChanged(int)), this, SLOT(changeK   (int)));
    connect(m_spinBoxK, SIGNAL(valueChanged(int)), this, SLOT(changeK   (int)));
    connect(m_sliderR , SIGNAL(valueChanged(int)), this, SLOT(changeRe  (int)));
    connect(m_spinBoxR, SIGNAL(valueChanged(int)), this, SLOT(changeRe  (int)));
    connect(m_radioButtonSort, SIGNAL(clicked() ), this, SLOT(changeMethod ()));
    connect(m_radioButtonHist, SIGNAL(clicked() ), this, SLOT(changeMethod ()));
    
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
    
    layout->addWidget(m_radioButtonSort , 3, 0);
    layout->addWidget(labelSortMethod   , 3, 1);
    layout->addWidget(m_radioButtonHist , 4, 0);
    layout->addWidget(labelHistoBased   , 4, 1);
    
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
    int nbrSz = m_sliderN->value();
    if (k > nbrSz*nbrSz/2) {
        k = nbrSz*nbrSz/2;  // max k is nbrSz*nbrSz/2
    }
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
MedianFilter::changeMethod() {
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

        int type;
        ChannelPtr<uchar> p1, p2, endd;
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            endd = p1+total;

            // copy first row to first nbr/2 buffers, where are top padded rows
            for (int i=0; i<nbr/2; i++) {
                copyRowToBuffer(p1, buffers[i], w, nbr);
                p1=p1-w; // we dont want to go to the next row, continue copying the first row
            }
            
            // continue to rest of buffers, note here we still begin copying from frist row of I1
            for (int i=nbr/2; i<nbr; i++) {
                copyRowToBuffer(p1, buffers[i], w, nbr);
            }

            clock_t t;
            if (m_radioButtonSort->isChecked()) {  // if sorting method is selected
                t = clock();  // start clock
                std::vector<int> v(0);  // vector for storing neighbors
                
                for (int y=0; y<h; y++) {  // visit each row
                    for (int i=0; i<nbr; i++) {  // visit each pixel value in neighborhood
                        for (int j=0; j<nbr; j++) {
                            v.push_back(buffers[j][i]);
                        }
                    }
                    
                    for (int x=0; x<w; x++) {  // visit each pixel in row
                        *p2++ = getMedianWithK(v, k);  // use sorting to find median
                        
                        if (x<w-1) { 
                            v.erase(v.begin(), v.begin()+nbr);  // delete outgoing column
                            for (int i=0; i<nbr; i++) {
                                v.push_back(buffers[i][x+nbr]);  // add incoming column
                            }
                        }
                    }
                    v.clear(); // clear vector

                    int nextRowIndex = y+nbr-1;
                    int nextBufferIndex = nextRowIndex%nbr;
                    copyRowToBuffer(p1, buffers[nextBufferIndex], w, nbr);
                    if (p1>endd) p1-=w; // if have passed last pix, go back to the first pix of last row
                }
                t = clock() - t;  // clock ends
                printf("Median filter %dx%d using sorting method is: %f seconds\n", nbr, nbr, ((float)t)/CLOCKS_PER_SEC);
                
            } else if (m_radioButtonHist->isChecked()) {   // if histogram based method is selected
                t = clock();   // start clock
                std::vector<int> histo(MXGRAY);
                
                for (int y=0; y<h; y++) {
                    for (int i=0; i<nbr; i++) {
                        for (int j=0; j<nbr; j++) {
                            histo[buffers[j][i]]++;
                        }
                    }
                    
                    for (int x=0; x<w-1; x++) {   //
                        *p2++ = getMedianHisto(histo, nbr*nbr, k);  // use histogram to find median

                        for (int i = 0; i < nbr; i++) {
                            histo[buffers[i][x]]--;      // delete outgoing column (x)
                            histo[buffers[i][x+nbr]]++;  // add incoming column (x+nbr)
                        }
                    }
                    *p2++ = getMedianHisto(histo, nbr*nbr, k);  // use histogram to find median
                    
                    std::fill(histo.begin(), histo.end(), 0); // reset histogram
                    
                    // move to the next row
                    int nextRowIndex = y+nbr-1; // index of next row that is needed to copy to buffer
                    int nextBufferIndex = nextRowIndex%nbr; // index of buffers that next row needs to copy to
                    copyRowToBuffer(p1, buffers[nextBufferIndex], w, nbr);
                    if (p1>endd) p1-=w; // if have passed last pix, go back to the first pix of last row
                }
                t = clock() - t;
                printf("Median filter %dx%d using histogram based is: %f seconds\n", nbr, nbr, ((float)t)/CLOCKS_PER_SEC);
            }
        }
    }
}

void
MedianFilter::copyRowToBuffer(ChannelPtr<uchar> &p1, short* bufFor1Row, int w, int nbr) {
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
    unsigned short sum;
    
    if (vSz==1) { return v[0]; }  // no need to sort if there is only 1 ele
    else {
        std::sort(v.begin(), v.end());  // sort the vector
        int middleIndex = vSz/2;    // middle index
        if (k==0) { return v[middleIndex]; }  // if k is 0 return the value at the middle index
        else {
            sum=v[middleIndex]; // initialize sum
            
            // add left k values and right k values to sum
            for (int i=1; i<=k; i++) { sum+= (v[middleIndex-i]+v[middleIndex+i]); }
            return sum/(k*2+1); // returns the average
        }
    }
}

int
MedianFilter::getMedianHisto(std::vector<int> histo, int total, int k) { // total is total # of neighbors
    int mid = total/2+1; // middle location of values in histo
    int count = 0;  // count number of pixels
    int i;
    for (i=0; i<MXGRAY; i++) {
        count += histo[i];
        if (count >= mid) {  // when count reaches to mid or exceeds, median is found
            break; // for loop breaks, now i is median
        }
    }
    if (k==0) { return i; }
    else {
        int sum = i;
        int left = mid-(count-histo[i])-1;  // how many on the left can give
        int right = count-mid;              // how many on the right can give
        int leftIndex=i, rightIndex=i;
        for (int j=1; j<=k; j++) {
            while (left == 0) {  // while left has none to give
                --leftIndex;    // move to one index to the left
                left=histo[leftIndex];  // left now has histo[leftIndex] to give
            }
            sum+=leftIndex; // give one to the sum
            --left; // left has one less to give
            while (right == 0) {
                ++rightIndex;
                right = histo[rightIndex];
            }
            sum+=rightIndex;
            --right;
        }
        
        return sum/(k*2+1);
    }
}

void
MedianFilter::reset() {
    changeNbr(1);
    changeK(0);
    changeRe(0);
    m_radioButtonSort->setChecked(true);
}
