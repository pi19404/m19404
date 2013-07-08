#-------------------------------------------------
#
# Project created by QtCreator 2013-07-06T12:05:11
#
#-------------------------------------------------

QT       += core gui


INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
# /media/UBUNTU/softwares-linux/FaceDetect_V1.3
LIBS += -L/usr/local/lib `pkg-config --libs opencv`


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HaarFeatures
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    time_measure.hpp

FORMS    += mainwindow.ui
