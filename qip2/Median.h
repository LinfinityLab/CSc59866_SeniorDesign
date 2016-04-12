//
//  Median.h
//  qip
//
//  Created by Weifan Lin on 4/11/16.
//
//

#ifndef Median_h
#define Median_h

#include "ImageFilter.h"


class Median : public ImageFilter {
    Q_OBJECT
    
public:
    Median	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void medianFilter(ImagePtr I1, int nbrSz, int k, ImagePtr I2);
    void copyToBuffer(ChannelPtr<uchar> &p1, int width, short* buffer, int bufSz);
    
    protected slots:
    void changeNbrSz(int);
    void changeK(int);
    
private:

    QSlider		*m_slider ;
    QSpinBox	*m_spinBox;
    
    QLabel		*m_label;
    
    QGroupBox	*m_ctrlGrp;	
    
};

#endif /* Median_h */
