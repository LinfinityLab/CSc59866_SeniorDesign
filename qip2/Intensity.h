//
//  Intensity.h
//  qip
//
//  Created by Weifan Lin on 3/17/16.
//
//

#ifndef Intensity_h
#define Intensity_h

#include "ImageFilter.h"

#define DEFAULTMAX	255
#define DEFAULTMIN	0
class Intensity : public ImageFilter {
    Q_OBJECT
    
public:
    Intensity   (QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void intensity(ImagePtr I1, int min, int max, ImagePtr I2);
    
    protected slots:
    void changeMin (int);
    void changeMax (int);
    void changeAuto(int);
    void autoSliderAndSpinBox(QSlider*, QSpinBox*, int);
    
private:
    // stretch controls
    QSlider		*m_sliderMin ;	// stretch sliders
    QSlider		*m_sliderMax ;	// stretch sliders
    
    QSpinBox	*m_spinBoxMin;	// stretch spin boxes
    QSpinBox	*m_spinBoxMax;	// stretch spin boxes

    QCheckBox   *m_checkBox;
    
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};




#endif /* Intensity_h */
