//
//  HistogramMatch.h
//  qip
//
//  Created by Weifan Lin on 3/29/16.
//
//

#ifndef HistogramMatch_h
#define HistogramMatch_h

#include "ImageFilter.h"


class HistogramMatch : public ImageFilter {
    Q_OBJECT
    
public:
    HistogramMatch   (QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void match(ImagePtr I1, int exp, ImagePtr I2);
    
    protected slots:
    void changeExp(int);
    
private:
    // match controls
    QSlider		*m_slider ;
    QSpinBox	*m_spinBox;
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};




#endif /* HistogramMatch_h */
