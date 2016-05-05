//
//  quantization.h
//  qip
//
//  Created by Weifan Lin on 3/14/16.
//
//

#ifndef quantization_h
#define quantization_h


#include "ImageFilter.h"


class Quantization : public ImageFilter {
    Q_OBJECT
    
public:
    Quantization	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void quantization(ImagePtr I1, int qtz, bool isDither, ImagePtr I2);
    
    protected slots:
    void changeQtz(int);
    void changeDtr(int);

    
private:
    // quantization controls
    QSlider		*m_slider ;	// quantization sliders
    QSpinBox	*m_spinBox;	// quantization spin boxes
    QCheckBox   *m_checkBox; 
    
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};


#endif /* quantization_h */