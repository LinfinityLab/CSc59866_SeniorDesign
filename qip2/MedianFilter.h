//
//  MedianFilter.h
//  qip
//
//  Created by Weifan Lin on 4/11/16.
//
//

#ifndef Median_h
#define Median_h

#include "ImageFilter.h"
#include <vector>


class MedianFilter : public ImageFilter {
    Q_OBJECT
    
public:
    MedianFilter	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void medianFilter(ImagePtr I1, int nbr, int k, ImagePtr I2);
    void copyRowToBuffer(ChannelPtr<uchar> &p1, short* buffer, int w, int nbr);
    int getMedianWithK(std::vector<int> v, int k);
    int getMedianHisto(std::vector<int> histo, int total, int k);
    
    protected slots:
    void changeNbr(int);
    void changeK(int);
    void changeRe(int);
    void changeMethod();
    
private:
    QSlider		 *m_sliderK ;
    QSpinBox	 *m_spinBoxK;
    QSlider      *m_sliderN;
    QSpinBox     *m_spinBoxN;
    QSlider      *m_sliderR;
    QSpinBox     *m_spinBoxR;
    QRadioButton *m_radioButtonSort;
    QRadioButton *m_radioButtonHist;
    QWindow *m_window;


    
    QLabel		*m_label;
    
    QGroupBox	*m_ctrlGrp;	
    
};

#endif /* Median_h */
