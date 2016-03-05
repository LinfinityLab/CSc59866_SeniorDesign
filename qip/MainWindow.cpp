// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2016 by George Wolberg
//
// MainWindow.cpp - MainWindow class
//
// Written by: George Wolberg, 2016
// ===============================================================

#include "MainWindow.h"
#include "Dummy.h"
#include "Threshold.h"
#include "Contrast.h"

enum {DUMMY, THRESHOLD, CONTRAST};
QString GroupBoxStyle = "QGroupBox {				\
			border: 2px solid gray;			\
			border-radius: 9px;			\
			margin-top: 0.5em;}";

MainWindow *g_mainWindowP = NULL;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::MainWindow:
//
// MainWindow constructor.
//
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  m_code(-1)
{
	setWindowTitle("Capstone Project");

	// set global variable for main window pointer
	g_mainWindowP = this;

	// INSERT YOUR ACTIONS AND MENUS
	createActions();
	createMenus  ();
	createWidgets();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createActions:
//
// Create actions to associate with menu and toolbar selection.
//
void
MainWindow::createActions()
{
	//////////////////////////////
	// File Actions
	//////////////////////////////

	m_actionOpen = new QAction("&Open", this);
	m_actionOpen->setShortcut(tr("Ctrl+O"));
	connect(m_actionOpen, SIGNAL(triggered()), this, SLOT(open()));

	m_actionQuit = new QAction("&Quit", this);
	m_actionQuit->setShortcut(tr("Ctrl+Q"));
	connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	//////////////////////////////
	// Point Ops Actions
	//////////////////////////////

	m_actionThreshold = new QAction("&Threshold", this);
	m_actionThreshold->setShortcut(tr("Ctrl+T"));
	m_actionThreshold->setData(THRESHOLD);

	m_actionContrast = new QAction("&Contrast", this);
	m_actionContrast->setShortcut(tr("Ctrl+C"));
	m_actionContrast->setData(CONTRAST);

	// one signal-slot connection for all actions;
	// execute() will resolve which action was triggered
	connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(execute(QAction*)));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createMenus:
//
// Create menus and install in menubar.
//
void
MainWindow::createMenus()
{
	// File menu
	m_menuFile = menuBar()->addMenu("&File");
	m_menuFile->addAction(m_actionOpen);
	m_menuFile->addAction(m_actionQuit);

	// Point Ops menu
	m_menuPtOps = menuBar()->addMenu("&Point Ops");
	m_menuPtOps->addAction(m_actionThreshold);
	m_menuPtOps->addAction(m_actionContrast );
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createWidgets:
//
// Create widgets for image display and filter control panels.
//
void
MainWindow::createWidgets()
{
	// assemble image display widget and control panel in horizontal layout
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(createGroupDisplay());
	hbox->setStretch(0, 1);
	hbox->addWidget(createGroupPanel());

	// create container widget and set its layout
	QWidget *w = new QWidget;
	w->setLayout(hbox);

	// set central widget so that it can be displayed
	setCentralWidget(w);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createGroupPanel:
//
// Create group box for control panel.
//
QGroupBox*
MainWindow::createGroupPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox;
	groupBox->setMinimumWidth(350);

	// filter's enum indexes into container of image filters
	m_imageFilterType[DUMMY	   ] = new Dummy;
	m_imageFilterType[THRESHOLD] = new Threshold;
	m_imageFilterType[CONTRAST ] = new Contrast;

	// create a stacked widget to hold multiple control panels
	m_stackWidgetPanels = new QStackedWidget;

	// add filter control panels to stacked widget
	m_stackWidgetPanels->addWidget(m_imageFilterType[DUMMY    ]->controlPanel());
	m_stackWidgetPanels->addWidget(m_imageFilterType[THRESHOLD]->controlPanel());
	m_stackWidgetPanels->addWidget(m_imageFilterType[CONTRAST ]->controlPanel());

	// display blank dummmy panel initially
	m_stackWidgetPanels->setCurrentIndex(0);

	// assemble display and mode groups into horizontal layout
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(createDisplayButtons());
	hbox->addWidget(createModeButtons   ());

	// assemble stacked widget in vertical layout
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addWidget(m_stackWidgetPanels);
	vbox->addStretch(1);
	vbox->addLayout(createExitButtons());
	groupBox->setLayout(vbox);

	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createGroupDisplay:
//
// Create group box for displaying images.
//
QGroupBox*
MainWindow::createGroupDisplay()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox;
	groupBox->setStyleSheet(GroupBoxStyle);

	// create stacked widget for input/output images
	m_stackWidgetImages = new QStackedWidget;

	// add QLabels to image stacked widget to display input/output images
	for(int i = 0; i<2; ++i)
		m_stackWidgetImages->addWidget(new QLabel);

	// add centering alignment on both labels
	QLabel *label;
	label = (QLabel *) m_stackWidgetImages->widget(0); label->setAlignment(Qt::AlignCenter);
	label = (QLabel *) m_stackWidgetImages->widget(1); label->setAlignment(Qt::AlignCenter);

	// set stacked widget to default setting: input image
	m_stackWidgetImages->setCurrentIndex(0);

	// assemble stacked widget in vertical layout
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_stackWidgetImages);
	groupBox->setLayout(vbox);

	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createDisplayButtons:
//
// Create preview window groupbox.
//
QGroupBox*
MainWindow::createDisplayButtons()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Display");

	// create radio buttons
	m_radioDisplay[0] = new QRadioButton("Input");
	m_radioDisplay[1] = new QRadioButton("Output");

	// create button group and add radio buttons to it
	QButtonGroup *bGroup = new QButtonGroup;
	for(int i = 0; i<2; ++i)
		bGroup->addButton(m_radioDisplay[i]);

	// set input radio button to be default
	m_radioDisplay[0]->setChecked(true);

	// assemble radio buttons into vertical widget
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_radioDisplay[0]);
	vbox->addWidget(m_radioDisplay[1]);
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_radioDisplay[0], SIGNAL(clicked()), this, SLOT(displayIn()));
	connect(m_radioDisplay[1], SIGNAL(clicked()), this, SLOT(displayOut()));

	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createModeButtons:
//
// Create preview window groupbox.
//
QGroupBox*
MainWindow::createModeButtons()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Mode");

	// create radio buttons
	m_radioMode[0] = new QRadioButton("RGB");
	m_radioMode[1] = new QRadioButton("Gray");

	// create button group and add radio buttons to it
	QButtonGroup *bGroup = new QButtonGroup;
	for(int i = 0; i<2; ++i)
		bGroup->addButton(m_radioMode[i]);

	// set RGB radio button to be default
	m_radioMode[0]->setChecked(true);

	// assemble radio buttons into vertical widget
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_radioMode[0]);
	vbox->addWidget(m_radioMode[1]);
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_radioMode[0], SIGNAL(clicked()), this, SLOT(modeRGB ()));
	connect(m_radioMode[1], SIGNAL(clicked()), this, SLOT(modeGray()));

	return groupBox;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createExitButtons:
//
// Create save/quit buttons.
//
QHBoxLayout*
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

//	m_imageFilterType[ m_imageFilterName[index] ]->reset();
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



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::open:
//
// Open input image.
//
void
MainWindow::open() {
	QFileDialog dialog(this);

	// open the last known working directory
	if(!m_currentDir.isEmpty())
		dialog.setDirectory(m_currentDir);

	// display existing files and directories
	dialog.setFileMode(QFileDialog::ExistingFile);

	// invoke native file browser to select file
	m_file = dialog.getOpenFileName(this,
					"Open File", m_currentDir,
					"Images (*.jpg *.png *.ppm *.pgm *.bmp);;All files (*)");

	// verify that file selection was made
	if(m_file.isNull()) return;

	// save current directory
	QFileInfo f(m_file);
	m_currentDir = f.absolutePath();

	// DISABLE IMAGING FUNCTIONALITY FOR NOW....
	// read input image
	m_imageIn = IP_readImage(qPrintable(m_file));

	if(m_radioMode[1]->isChecked())
		IP_castImage(m_imageIn,  BW_IMAGE, m_imageSrc);
	else	IP_castImage(m_imageIn, RGB_IMAGE, m_imageSrc);

	// init vars
	m_width  = m_imageSrc->width ();
	m_height = m_imageSrc->height();
	preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::display:
//
// Slot functions to display input and output images.
//
void MainWindow::displayIn () { display(0); }
void MainWindow::displayOut() { display(1); }

void MainWindow::display(int flag)
{
	// error checking
	if(m_imageSrc.isNull())		return;		// no input  image
	if(m_imageDst.isNull() && flag) return;		// no output image

	// raise the appropriate widget from the stack
	m_stackWidgetImages->setCurrentIndex(flag);

	// set radio button
	m_radioDisplay[flag]->setChecked(1);

	// determine image to be displayed
	ImagePtr I;
	if(flag == 0)
		I = m_imageSrc;
	else	I = m_imageDst;

	// init image dimensions
	int  w = m_imageDst->width();
	int  h = m_imageDst->height();

	// init view window dimensions
	int ww = m_stackWidgetImages->width();
	int hh = m_stackWidgetImages->height();

	// convert from ImagePtr to QImage to Pixmap
	QImage q;
	IP_IPtoQImage(I, q);

	// convert from QImage to Pixmap; rescale if image is larger than view window
	QPixmap p;
	if(MIN(w, h) > MIN(ww, hh))
		p = QPixmap::fromImage(q.scaled(QSize(ww, hh), Qt::KeepAspectRatio));
	else	p = QPixmap::fromImage(q);

	// assign pixmap to label widget for display
	QLabel *widget = (QLabel *) m_stackWidgetImages->currentWidget();
	widget->setPixmap(p);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::mode:
//
// Slot functions to display RGB and grayscale images.
//
void MainWindow::modeRGB () { mode(0); }
void MainWindow::modeGray() { mode(1); }

void MainWindow::mode(int flag)
{
	// error checking
	if(m_imageSrc.isNull()) return;		// no input image

	if(flag)
		IP_castImage(m_imageIn,  BW_IMAGE, m_imageSrc);
	else	IP_castImage(m_imageIn, RGB_IMAGE, m_imageSrc);

	// re-apply filter for changed mode
	if(m_code > 0)
		m_imageFilterType[m_code]->applyFilter(m_imageSrc, m_imageDst);

	// display image
	preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::preview:
//
// Display preview image.
//
void
MainWindow::preview()
{
	// display requested image
	if(m_radioDisplay[0]->isChecked())
		display(0);
	else	display(1);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::execute:
//
// Determine which action was triggered and execute respective action.
// Skip this if action is not image-processing related: open(), quit()
//
void
MainWindow::execute(QAction* action)
{
	// skip over menu ops that don't require image processing
	QString name = action->text();
	if(name == QString("&Open") || name == QString("&Quit")) {
		m_code = -1;
		return;
	}

	// get code from action
	m_code = action->data().toInt();

	// set output radio button to true
	m_radioDisplay[1]->setChecked(true);

	// use code to index into stack widget and array of filters
	m_stackWidgetPanels->setCurrentIndex(m_code);
	m_imageFilterType[m_code]->applyFilter(m_imageSrc, m_imageDst);
	preview();
}
