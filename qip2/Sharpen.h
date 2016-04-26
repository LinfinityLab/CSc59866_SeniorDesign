//
//  Sharpen.h
//  qip
//
//  Created by Weifan Lin on 4/12/16.
//
//

#ifndef Sharpen_h
#define Sharpen_h

#include "ImageFilter.h"
#include "Blur.h"

class Sharpen : public Blur {
    Q_OBJECT
    
public:
    Sharpen	    (QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void edge(ImagePtr I1, int fctr, ImagePtr I2);
    void shrp(ImagePtr I1, int fctr, ImagePtr I2);
    
    protected slots:
    void changeEdge();
    void changeShrp();
    void changeFctr(int);
    
private:
    QSlider		*m_sliderFctr  ;
    QSpinBox	*m_spinBoxFctr ;
    QRadioButton *m_radioButtonShrp;
    QRadioButton *m_radioButtonEdge;
    
};

#endif /* Sharpen_h */
