//
//  ErrorDiffusion.h
//  qip
//
//  Created by Weifan Lin on 3/30/16.
//
//

#ifndef ErrorDiffusion_h
#define ErrorDiffusion_h

#include "ImageFilter.h"


class ErrorDiffusion : public ImageFilter {
    Q_OBJECT
    
public:
    ErrorDiffusion	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void errorDiffusion(ImagePtr I1, int thr, ImagePtr I2);
    void copyToBuffer(ChannelPtr<uchar> &p1, int width, short* buffer, int bufSz);
    
    protected slots:
    void changeThr(int);
    
private:
    // threshold controls
    QSlider		*m_slider ;	// Threshold sliders
    QSpinBox	*m_spinBox;	// Threshold spin boxes
    
    // label for Otsu thresholds
    QLabel		*m_label;	// Label for printing Otsu thresholds
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
    
};



#endif /* ErrorDiffusion_h */

