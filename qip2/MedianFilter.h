//
//  MedianFilter.h
//  qip
//
//  Created by Weifan Lin on 4/11/16.
//
//

#ifndef Median_h
#define Median_h

#include "ImageFilter.h"
#include <vector>


class MedianFilter : public ImageFilter {
    Q_OBJECT
    
public:
    MedianFilter	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void medianFilter(ImagePtr I1, int nbr, int k, ImagePtr I2);
    void copyOneRowToBuffer(ChannelPtr<uchar> &p1, short* buffer, int w, int nbr);
    //int getMedianWithK(int arr[], int arrSz, int k);
    int getMedianWithK(std::vector<int> v, int k);
    void IP_padImage(ImagePtr src, int padSz, ImagePtr padded);
//    int storeAllNbrToArr(int arr[], short* buffers[], int index);
    
    protected slots:
    void changeNbr(int);
    void changeK(int);
    void changeRe(int);
    
private:
    QSlider		*m_sliderK ;
    QSpinBox	*m_spinBoxK;
    QSlider     *m_sliderN;
    QSpinBox    *m_spinBoxN;
    QCheckBox   *m_checkBox;
    
    QLabel		*m_label;
    
    QGroupBox	*m_ctrlGrp;	
    
};

#endif /* Median_h */
