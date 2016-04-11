//
//  Blur.h
//  qip
//
//  Created by Weifan Lin on 4/4/16.
//
//

#ifndef Blur_h
#define Blur_h

#include "ImageFilter.h"


class Blur : public ImageFilter {
    Q_OBJECT
    
public:
    Blur	    (QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void blur(ImagePtr I1, int xsz, int ysz, ImagePtr I2);
    void edge(ImagePtr I1, ImagePtr I2);
    void shrp(ImagePtr I1, int fctr, ImagePtr I2);
    void IP_blur1D(ChannelPtr<uchar> &p1, int size, int kernel, int stride, ChannelPtr<uchar> &p3);
    
    protected slots:
    void changeXsz (int);
    void changeYsz (int);
    void changeSync(int);
    void changeEdge(int);
    void changeShrp(int);
    void changeFctr(int);
    
private:
    QSlider		*m_sliderX     ;
    QSlider		*m_sliderY     ;
    QSpinBox	*m_spinBoxX    ;
    QSpinBox	*m_spinBoxY    ;
    QCheckBox   *m_checkBoxSync;
    QCheckBox   *m_checkBoxEdge;
    QCheckBox   *m_checkBoxShrp;
    QSlider		*m_sliderFctr  ;
    QSpinBox	*m_spinBoxFctr ;
    
    QLabel		*m_label;
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// Groupbox for panel
};

#endif /* Blur_h */
