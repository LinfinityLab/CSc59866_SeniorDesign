// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// MainWindow.h - Header file for MainWindow class
//
// Written by: George Wolberg, 2015
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
#include "HW.h"

typedef std::map<QString, HW*> hw_type;

class MainWindow : public QWidget {
	Q_OBJECT

public:
	// constructor
	MainWindow	(QWidget *parent = 0);


public slots:
	void		createWidgets	();
	void		changeTab	(int);
	void		reset		();
	void		quit		();

protected:
	QGroupBox*	createGroupView	  ();
	QGroupBox*	createGroupInput  ();
	QHBoxLayout*	createExitButtons ();

private:
	// homework objects
	QStringList	 m_hwName;
	hw_type		 m_hw;

	// widgets
	QTabWidget	*m_tabWidget;
	QStackedWidget	*m_stackWidget;
};

extern MainWindow *MainWindowP;

#endif // MAINWINDOW_H
