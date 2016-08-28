// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// MainWindow.cpp - MainWindow class
//
// Written by: George Wolberg, 2015
// ===============================================================

#include "MainWindow.h"
#include "HW0a.h"
#include "HW0b.h"
#include "HW0c.h"

QString GroupBoxStyle = "QGroupBox {				\
			border: 2px solid gray;			\
			border-radius: 9px;			\
			margin-top: 0.5em;}";

MainWindow *MainWindowP = NULL;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::MainWindow:
//
// MainWindow constructor.
//
MainWindow::MainWindow(QWidget *parent)
	:  QWidget(parent)
{
	setWindowTitle("Computer Graphics Homework");

	// set global variable for main window pointer
	MainWindowP = this;

	// create a stacked widget to hold multiple control panels
	m_stackWidget = new QStackedWidget;

	// INSERT YOUR OBJECTS IN THIS FUNCTION
	createWidgets();

	// add stacked widget to vertical box layout 
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_stackWidget);
	vbox->addStretch(1);
	vbox->addLayout(createExitButtons());

	// add all widgets to grid layout
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(createGroupView());
	hbox->setStretch(0, 1);
	hbox->addLayout(vbox);
	setLayout(hbox);

	// set stacked widget to first solution
	m_tabWidget->setCurrentIndex(0);
	m_stackWidget->setCurrentIndex(0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createWidgets:
//
// Create user-defined widgets for display and control panel.
// INSERT YOUR LINES HERE TO CREATE A TAB PER HOMEWORK OBJECT.
//
void
MainWindow::createWidgets()
{
	// create list of hw names; m_hwName name will be used for
	// tab name and as key for class in m_hw container
	m_hwName << "HW 0a" << "HW 0b" << "HW 0c";

	// instantiate homework solution classes
	m_hw[m_hwName[0]] = new HW0a;
	m_hw[m_hwName[1]] = new HW0b;
	m_hw[m_hwName[2]] = new HW0c;

	// add control panels to stacked widget
	for(int i = 0; i < (int) m_hwName.size(); i++)
		m_stackWidget->addWidget(m_hw[ m_hwName[i] ]->controlPanel());
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createGroupView:
//
// Create preview window groupbox.
//
QGroupBox*
MainWindow::createGroupView()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox();
	groupBox->setStyleSheet(GroupBoxStyle);

	// create a tab widget to handle multiple displays
	m_tabWidget = new QTabWidget;

	// add one tab per homework problem
	for(int i = 0; i < (int) m_hwName.size(); i++)
		m_tabWidget->addTab(m_hw[m_hwName[i]], m_hwName[i]);

	// assemble stacked widget in vertical layout
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_tabWidget);
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));

	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::changeTab:
//
// Slot function to change OpenGL canvas and control panel in stacked widget.
//
void
MainWindow::changeTab(int index)
{
	m_tabWidget  ->setCurrentIndex(index);	// change OpenGL widget 
	m_stackWidget->setCurrentIndex(index);	// change control panel

	// change keyboard focus to GL widget
	m_hw[m_hwName[index]]->setFocusPolicy(Qt::StrongFocus);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createExitButtons:
//
// Create save/quit buttons.
//
QHBoxLayout *
MainWindow::createExitButtons()
{
	// create pushbuttons
	QPushButton *buttonReset = new QPushButton("Reset");
	QPushButton *buttonQuit  = new QPushButton("Quit");

	// init signal/slot connections
	connect(buttonReset, SIGNAL(clicked()), this, SLOT(reset()));
	connect(buttonQuit , SIGNAL(clicked()), this, SLOT(quit ()));

	// assemble pushbuttons in horizontal layout
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(buttonReset);
	buttonLayout->addWidget(buttonQuit );

	return buttonLayout;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::reset:
//
// Reset application.
//
void
MainWindow::reset()
{
	int index = m_tabWidget->currentIndex();	// current OpenGL widget
	m_hw[ m_hwName[index] ]->reset();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::quit:
//
// Quit application.
//
void
MainWindow::quit()
{
	// close the dialog window
	close();
}

