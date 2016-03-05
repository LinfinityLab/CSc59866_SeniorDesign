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
#include <vector>
#include <map>
#include <algorithm>
#include "IP.h"
#include "IPtoUI.h"
#include "ImageFilter.h"

#define MAXFILTERS	50

using namespace IP;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	// constructor
	MainWindow	(QWidget *parent = 0);
	ImagePtr	imageSrc	() const;
	ImagePtr	imageDst	() const;

public slots:
	void		open		();
	void		displayIn	();
	void		displayOut	();
	void		modeRGB		();
	void		modeGray	();
	void		preview		();
	void		reset		();
	void		quit		();
	void		execute		(QAction*);

protected:
	void		createActions	();
	void		createMenus	();
	void		createWidgets	();
	QGroupBox*	createGroupPanel();
	QGroupBox*	createGroupDisplay  ();
	QGroupBox*	createDisplayButtons();
	QGroupBox*	createModeButtons();
	QHBoxLayout*	createExitButtons();
	void		display		(int);
	void		mode		(int);


private:
	QMenu*			m_menuFile;
	QMenu*			m_menuPtOps;
	QAction*		m_actionOpen;
	QAction*		m_actionQuit;
	QAction*		m_actionThreshold;
	QAction*		m_actionContrast ;

	// homework objects
	ImageFilter*		m_imageFilterType[MAXFILTERS];

	// widgets
	QStackedWidget*		m_stackWidgetImages;
	QStackedWidget*		m_stackWidgetPanels;

	// widgets for image display groupbox
	QRadioButton*		m_radioDisplay[2];
	QRadioButton*		m_radioMode   [2];

	int			m_width;
	int			m_height;
	int			m_code;
	QString			m_file;
	QString			m_currentDir;
	ImagePtr		m_imageIn;
	ImagePtr		m_imageSrc;
	ImagePtr		m_imageDst;
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
