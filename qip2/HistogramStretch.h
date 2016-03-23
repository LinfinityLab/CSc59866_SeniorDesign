//
//  HistogramStretch.h
//  qip
//
//  Created by Weifan Lin on 3/17/16.
//
//

#ifndef HistogramStretch_h
#define HistogramStretch_h

#include "ImageFilter.h"

#define DEFAULTMAX	255
#define DEFAULTMIN	0
class HistogramStretch : public ImageFilter {
    Q_OBJECT
    
public:
    HistogramStretch   (QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void stretch(ImagePtr I1, int min, int max, ImagePtr I2);
    
    protected slots:
    void changeMin (int);
    void changeMax (int);
    void autoMin   (int);
    void autoMax   (int);
    void autoSliderAndSpinBox(QSlider*, QSpinBox*, int);
    
private:
    // stretch controls
    QSlider		*m_sliderMin ;	// stretch sliders
    QSlider		*m_sliderMax ;	// stretch sliders
    
    QSpinBox	*m_spinBoxMin;	// stretch spin boxes
    QSpinBox	*m_spinBoxMax;	// stretch spin boxes

    QCheckBox   *m_checkBoxMin;
    QCheckBox   *m_checkBoxMax;
    
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};




#endif /* HistogramStretch_h */
