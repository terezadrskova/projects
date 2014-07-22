#-------------------------------------------------
#
# Project created by QtCreator 2014-06-15T14:38:35
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

RESOURCES   = draggableicons.qrc

 # install image resources
 target.path = $$[QT_INSTALL_EXAMPLES]/editing-gui
 sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro images
 sources.path = $$[QT_INSTALL_EXAMPLES]/editing-gui
 INSTALLS += target sources

SOURCES += main.cpp\
        mainwindow.cpp \
    loadPPM.cpp \
    voronoi-diagram.cpp \
    loading-images.cpp \
    directions.cpp \
    load-directions-and-intensities.cpp \
    intensities.cpp \
    math-functions.cpp \
    gamma.cpp \
    relighting.cpp \
    image-manipulation.cpp \
    lightsource.cpp \
    lights.cpp

HEADERS  += mainwindow.h \
    loadPPM.h \
    lightsource.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/opencv
INCLUDEPATH += /usr/include/opencv2

LIBS += -L/usr/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lglut -lGLU
