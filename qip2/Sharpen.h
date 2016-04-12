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

class Sharpen : public ImageFilter {
    Q_OBJECT
    
public:
    Sharpen	    (QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void blur(ImagePtr I1, int xsz, int ysz, ImagePtr I2);
    void edge(ImagePtr I1, int fctr, ImagePtr I2);
    void shrp(ImagePtr I1, int fctr, ImagePtr I2);
    void IP_blur1D(ChannelPtr<uchar> &p1, int size, int kernel, int stride, ChannelPtr<uchar> &p3);
    
    protected slots:
    void changeXsz (int);
    void changeYsz (int);
    void changeSync(int);
    void changeEdge();
    void changeShrp();
    void changeFctr(int);
    
private:
    QSlider		*m_sliderX     ;
    QSlider		*m_sliderY     ;
    QSpinBox	*m_spinBoxX    ;
    QSpinBox	*m_spinBoxY    ;
    QCheckBox   *m_checkBoxSync;
    QSlider		*m_sliderFctr  ;
    QSpinBox	*m_spinBoxFctr ;
    QRadioButton *m_radioButtonShrp;
    QRadioButton *m_radioButtonEdge;
    
    QLabel		*m_label;
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};

#endif /* Sharpen_h */
