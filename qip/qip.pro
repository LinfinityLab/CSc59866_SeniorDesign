TEMPLATE = app
TARGET = qip
QT += widgets 
OBJECTS_DIR = ./obj
MOC_DIR     = ./moc


win32-msvc2013 {
	INCLUDEPATH 	+= ./IP/win/header
	LIBS 		+= -L./IP/win/lib
	LIBS 		+= -lopengl32 -lIP_d
	QMAKE_CXXFLAGS += /MP /Zi
}


macx{
        QMAKE_MAC_SDK = macosx10.11
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
	INCLUDEPATH += ./IP/mac/header
	LIBS += -L./IP/mac/lib
	LIBS += -lIP_d
}

unix:!macx {
	CONFIG += C++11
	INCLUDEPATH += ./IP/linux/header
	LIBS += -L./IP/linux/lib
	LIBS += -lIP_d
}

mingw {
	LIBS += -lopengl32
}

# Input
HEADERS +=	MainWindow.h	\
		ImageFilter.h	\
		Dummy.h		\
		Threshold.h	\
		Contrast.h

		
SOURCES +=	main.cpp	\ 
		MainWindow.cpp 	\
		ImageFilter.cpp	\
		Dummy.cpp	\
		Threshold.cpp	\
		Contrast.cpp

