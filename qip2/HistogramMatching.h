//
//  HistogramMatching.h
//  qip
//
//  Created by Weifan Lin on 3/29/16.
//
//

#ifndef HistogramMatching_h
#define HistogramMatching_h

#include "ImageFilter.h"


class HistogramMatching : public ImageFilter {
    Q_OBJECT
    
public:
    HistogramMatching   (QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void histogramMatching(ImagePtr I1, int exp, ImagePtr I2);
    
    protected slots:
    void changeExp(int);
    
private:
    // match controls
    QSlider		*m_slider ;
    QSpinBox	*m_spinBox;
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};




#endif /* HistogramMatching_h */
