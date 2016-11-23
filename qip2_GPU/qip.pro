TEMPLATE    = app
TARGET      = qip
QT 	   += widgets printsupport opengl
RESOURCES   = qip.qrc
CONFIG     += qt debug_and_release


Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR     = release/.moc
Debug:OBJECTS_DIR   = debug/.obj
Debug:MOC_DIR       = debug/.moc


win32-msvc2015 {
	Release:DESTDIR = release
	Debug:DESTDIR = debug
	INCLUDEPATH 	+= ./IP/win/header
	LIBS 		+= -L./IP/win/lib
	CONFIG(release, debug|release) {
		LIBS += -lIP
	} else {
		LIBS += -lIP_d 
	}
	LIBS 		+= -lopengl32
	QMAKE_CXXFLAGS  += /MP /Zi
}

macx{
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
	INCLUDEPATH += ./IP/mac/header
	LIBS        += -L./IP/mac/lib
	CONFIG(release, debug|release) {
		LIBS += -lIP
	} else {
		LIBS += -lIP_d 
	}
}

unix:!macx {

	CONFIG += C++11
	INCLUDEPATH += ./IP/linux/header
	LIBS        += -L./IP/linux/lib
	LIBS        += -lIP_d
}


# Input
HEADERS +=	MainWindow.h	\
		ImageFilter.h	\
		qcustomplot.h	\
		Dummy.h		\
		Threshold.h	\
		Clip.h		\
		Quantize.h	\
		Gamma.h		\
		Contrast.h	\
		HistoStretch.h	\
		HistoMatch.h	\
		ErrDiffusion.h	\
		Blur.h		\
		Sharpen.h	\
		Median.h	\
		GLWidget.h	\
		Convolve.h	

		
SOURCES +=	main.cpp	\ 
		MainWindow.cpp 	\
		ImageFilter.cpp	\
		qcustomplot.cpp	\
		Dummy.cpp	\
		Threshold.cpp	\
		Clip.cpp	\
		Quantize.cpp	\
		Gamma.cpp	\
		Contrast.cpp	\
		HistoStretch.cpp\
		HistoMatch.cpp	\
		ErrDiffusion.cpp\
		Blur.cpp	\
		Sharpen.cpp	\
		Median.cpp	\
		GLWidget.cpp	\
		Convolve.cpp	
