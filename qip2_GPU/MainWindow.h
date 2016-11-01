// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2016 by George Wolberg
//
// MainWindow.h - Header file for MainWindow class
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// ----------------------------------------------------------------------
// standard include files
//
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include "IP.h"
#include "IPtoUI.h"
#include "ImageFilter.h"
#include "qcustomplot.h"
#include "GLWidget.h"

#define MAXFILTERS	50
enum {
	DUMMY, THRESHOLD, CLIP, QUANTIZE, GAMMA, CONTRAST, HISTOSTRETCH, HISTOMATCH,
	ERRDIFFUSION, BLUR, SHARPEN, MEDIAN, CONVOLVE
};
using namespace IP;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	// constructor
	MainWindow	(QWidget *parent = 0);
	ImagePtr	imageSrc	() const;
	ImagePtr	imageDst	() const;
	void		setImageDst	(ImagePtr I) {
		if(m_radioMode[1]->isChecked())
			IP_castImage(I, BW_IMAGE, I);
		m_imageDst = I;
	}
	QCustomPlot*	histogram	()      
		{return m_histogram;}
	GLWidget*	glw		()      
		{return m_glw;}
	void		preview		();
	bool		gpuFlag		()      
		{ return m_checkboxGPU->checkState() ==  Qt::Checked; }
	bool		timeFlag	()	
		{ return m_checkboxTime->checkState() ==  Qt::Checked; }
	void		gpuProgram      (int pass) 
		{ m_imageFilter[m_code]->gpuProgram(pass); }
	int		gpuPasses () 
		{ return m_imageFilter[m_code]->gpuPasses(); }
	ImageFilter*	imageFilter(int i)	
		{ return m_imageFilter[i]; }
	bool		isInput		()	
		{ return m_radioDisplay[0]->isChecked();}

public slots:
	void		open		();
	void		save		();
	void		displayIn	();
	void		displayOut	();
	void		modeRGB		();
	void		modeGray	();
	void		reset		();
	void		quit		();
	void		time		();
	void		execute		(QAction*);


protected slots:
	void		setHisto	(int);
	void		setTime		(int);
	void		setGPU		(int);

protected:
	void		createActions	();
	void		createMenus	();
	void		createWidgets	();
	QGroupBox*	createGroupPanel();
	QFrame*		createGroupDisplay  ();
	QGroupBox*	createDisplayButtons();
	QGroupBox*	createModeButtons   ();
	QGroupBox*	createOptionButtons ();
	QHBoxLayout*	createExitButtons   ();
	void		displayHistogram(ImagePtr);
	void		display		(int);
	void		mode		(int);


private:
	// menus
	QMenu*			m_menuFile;
	QMenu*			m_menuPtOps;
	QMenu*			m_menuNbrOps;

	// point ops actions
	QAction*		m_actionOpen;
	QAction*		m_actionSave;
	QAction*		m_actionQuit;
	QAction*		m_actionThreshold   ;
	QAction*		m_actionClip	    ;
	QAction*		m_actionQuantize    ;
	QAction*		m_actionGamma	    ;
	QAction*		m_actionContrast    ;
	QAction*		m_actionHistoStretch;
	QAction*		m_actionHistoMatch  ;

	// neighborhood ops actions
	QAction*		m_actionErrDiffusion;
	QAction*		m_actionBlur	    ;
	QAction*		m_actionSharpen	    ;
	QAction*		m_actionMedian	    ;
	QAction*		m_actionConvolve    ;


	// homework objects
	ImageFilter*		m_imageFilter[MAXFILTERS];

	// widgets for control panel groupbox
	QGroupBox*		m_groupBoxPanels;	// group box for control panel
	QStackedWidget*		m_stackWidgetPanels;	// stacked widget for control panels

	// widgets for image display groupbox
	QFrame*			m_glwFrame;		// pointer to frame that holds glwidget
	QRadioButton*		m_radioDisplay[2];	// radio buttons for input/output
	QRadioButton*		m_radioMode   [2];	// radio buttons for RGB/Gray modes
	QCheckBox*		m_checkboxHisto;	// checkbox: histogram display
	QCheckBox*		m_checkboxTime;		// checkbox: compute timings
	QCheckBox*		m_checkboxGPU;		// checkbox: GPU acceleration
	QLabel*			m_labelTime;		// label for timing
	QWidget*		m_extension;		// extension widget for histogram
	QCustomPlot*		m_histogram;		// histogram plot

	int			m_width;		// image width
	int			m_height;		// image height
	int			m_code;			// code (index) of triggered action
	QString			m_file;			// input image filename
	QString			m_currentDir;		// input image directory
	ImagePtr		m_imageIn;		// input image (raw)
	ImagePtr		m_imageSrc;		// input image (processed)
	ImagePtr		m_imageDst;		// output image

	// histogram variables
	int			m_histoColor;		// histo color id: 0=RGB,1=R,2=G,3=B,4=gray
	double			m_histoXmin[4];		// xmin for all histogram channels
	double			m_histoXmax[4];		// xmax for all histogram channels
	double			m_histoYmin[4];		// ymin for all histogram channels
	double			m_histoYmax[4];		// ymax for all histogram channels
	GLWidget	       *m_glw;
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::imageSrc:
//
// Source image.
//
inline ImagePtr
MainWindow::imageSrc() const
{
	return m_imageSrc;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::imageDst:
//
// Destination image.
//
inline ImagePtr
MainWindow::imageDst() const
{
	return m_imageDst;
}

#endif // MAINWINDOW_H
